//#pragma GCC target ("avx2")
//#pragma GCC optimization ("O3")
//#pragma GCC optimization ("unroll-loops")

#include<iostream>
#include<vector>
#include<array>
#include<limits>
#include<algorithm>
#include<set>
#include<cmath>
#include<unordered_set>

using namespace std;

//#define LITTLE

namespace schema {
    enum dimension_type {
        IP, PORT
    };
    unsigned int n_dims;

    const unsigned int MAXD = 5;
    array<dimension_type, MAXD> dims;

    unsigned int best_dim = 1;
}

namespace data {
#ifdef LITTLE
    const unsigned int IP_SIZE = 5;
#else
    const unsigned int IP_SIZE = 32;
#endif
    unsigned int n, m;
    const unsigned int MAXN = 150000;
    const unsigned int MAXM = 15000;

    array<array<pair<unsigned int, unsigned int>, schema::MAXD>, MAXN> rules;
    array<array<unsigned int, schema::MAXD>, MAXM> keys;
    array<int, MAXM> ans;
}

namespace utils {
    bool match(const array<pair<unsigned int, unsigned int>, schema::MAXD> &rule,
               const array<unsigned int, schema::MAXD> &key) {
        for (unsigned int i = 0; i < schema::best_dim; i++)
            if (key[i] < rule[i].first or key[i] > rule[i].second)
                return false;

        for (unsigned int i = schema::best_dim + 1; i < schema::MAXD; i++)
            if (key[i] < rule[i].first or key[i] > rule[i].second)
                return false;
        return true;
    }
}

namespace parsing {
    unsigned int parse_key_token(const string &token, const schema::dimension_type &type) {
        unsigned int ret = 0;
        if (type == schema::dimension_type::IP)
            ret = (unsigned int) stoul(token, nullptr, 2);
        else
            ret = (unsigned int) stoul(token);
        return ret;
    }

    array<pair<unsigned int, unsigned int>, schema::MAXD>
    parse_rule(const vector<string> &tokens) {
        array<pair<unsigned int, unsigned int>, schema::MAXD> ranges;
        for (unsigned int i = 0; i < schema::n_dims; ++i) {
            ranges[i] = {0, 0};
            const auto &token = tokens[i];
            const auto &type = schema::dims[i];
            auto &l = ranges[i].first;
            auto &r = ranges[i].second;
            if (type == schema::dimension_type::IP) {
                for (unsigned int j = 0; j < data::IP_SIZE; ++j) {
                    const char c = token[j];
                    if (c == '*') {
                        l <<= (data::IP_SIZE - j);
                        r <<= (data::IP_SIZE - j);
                        r |= ((unsigned int)~0) >> j;
                        break;
                    } else {
                        l <<= 1;
                        l |= (c - '0');
                        r <<= 1;
                        r |= (c - '0');
                    }
                }
            } else {
                unsigned int cur = 0;
                while (token[cur] != '-') {
                    l = l * 10 + token[cur] - '0';
                    cur++;
                }
                cur++;
                while (cur < token.size() && token[cur] != '\r') {
                    r = r * 10 + token[cur] - '0';
                    cur++;
                }
            }
        }
        for (unsigned int i = schema::n_dims; i < schema::MAXD; i++)
            ranges[i] = {0, numeric_limits<unsigned int>::max()};
        return ranges;
    }

    array<unsigned int, schema::MAXD>
    parse_key(const vector<string> &tokens) {
        array<unsigned int, schema::MAXD> ret{};
        for (unsigned int i = 0; i < schema::n_dims; ++i)
            ret[i] = parse_key_token(tokens[i], schema::dims[i]);
        return ret;
    }

    void parse_input() {
        ios_base::sync_with_stdio(false);
        cin.tie(nullptr);
        cout.tie(nullptr);
#ifdef LITTLE
        freopen("little_input.txt", "r", stdin);
#else
        freopen("input.txt", "r", stdin);
#endif
        freopen("output.txt", "w", stdout);

        cin >> data::n;
        if (data::n == 0) {
            cerr << "no data is read, you probably fucked up" << endl;
            return;
        }
        {
            vector<string> tokens;
            {
                string line;

                cin.ignore();
                cin.ignore();

                // fgetsn
                getline(cin, line);
                size_t r = 0;
                while ((r = line.find(' ')) != string::npos) {
                    tokens.push_back(line.substr(0, r));
                    line.erase(0, r + 1);
                }
                tokens.push_back(line);
            }
            for (const auto &token: tokens) {
                if (token.find('-') == string::npos)
                    schema::dims[schema::n_dims++] = schema::dimension_type::IP;
                else
                    schema::dims[schema::n_dims++] = schema::dimension_type::PORT;
            }
            data::rules[0] = parse_rule(tokens);
        }

        for (unsigned int i = 1; i < data::n; i++) {
            vector<string> tokens(schema::n_dims);
            for (unsigned int j = 0; j < schema::n_dims; j++)
                cin >> tokens[j];
            data::rules[i] = parse_rule(tokens);
        }

        cin >> data::m;

        for (unsigned int i = 0; i < data::m; i++) {
            vector<string> tokens(schema::n_dims);
            for (unsigned int j = 0; j < schema::n_dims; j++)
                cin >> tokens[j];
            data::keys[i] = parse_key(tokens);
        }
    }
}

namespace logic {
    struct event {
        unsigned int loc, id;
        enum type {
            OPEN, KEY, CLOSE
        } type;

        bool operator<(const logic::event &other) {
            if (loc == other.loc)
                return type < other.type;
            return loc < other.loc;
        }
    };

    array<event, 2u * data::MAXN + data::MAXM> evt;
    unsigned int n_evt;
}

int main() {
    parsing::parse_input();

    data::ans.fill(-1);

    for (unsigned int i = 0; i < schema::n_dims; i++) {
        if (schema::dims[i] == schema::dimension_type::IP) {
            schema::best_dim = i;
            break;
        }
    }
//    {
//        unsigned long long best_n_operations = numeric_limits<unsigned long long>::max();
//        for (int dim = 0; dim < schema::n_dims; dim++) {
//            if (schema::dims[dim] == schema::dimension_type::PORT)
//                continue;
//            vector<logic::event> evt;
//            for (unsigned int i = 0; i < data::m; i++)
//                evt.push_back(logic::event{data::keys[i][dim], i, logic::event::type::KEY});
//            for (unsigned int i = 0; i < data::n; i++) {
//                evt.push_back(logic::event{data::rules[i][dim].first, i, logic::event::type::OPEN});
//                evt.push_back(logic::event{data::rules[i][dim].second, i, logic::event::type::CLOSE});
//            }
//            sort(evt.begin(), evt.end());
//
//            unsigned long long n_operations = 0;
//            unsigned int opened = 0;
//            vector<unsigned int> stats;
//            set<int> open_rules;
//            set<int> non_trivial;
//            for (const auto &e: evt) {
//                if (e.type == logic::event::type::OPEN) {
//                    opened++;
//                    open_rules.insert(e.id);
//                }
//                if (e.type == logic::event::type::KEY) {
//                    stats.push_back(opened);
//                    n_operations += opened;
//                    if (not open_rules.empty())
//                        non_trivial.insert(*open_rules.begin());
//                    if (n_operations > best_n_operations)
//                        break;
//                }
//                if (e.type == logic::event::type::CLOSE) {
//                    opened--;
//                    open_rules.erase(e.id);
//                }
//            }
//            if (n_operations < best_n_operations) {
//                best_n_operations = n_operations;
//                schema::best_dim = dim;
//            }
//            {
//                cerr << "dimension: " << dim << endl;
//                double mean = 0.0;
//                for (auto it : stats)
//                    mean += it;
//                mean /= stats.size();
//                double std = 0.0;
//                for (auto it : stats)
//                    std += (mean - it) * (mean - it);
//                std /= stats.size();
//                std = sqrt(std);
//                cerr << "size: " << stats.size() << ", total: " << n_operations << ", mean: " << mean << ", std: " << std << endl;
//                cerr << non_trivial.size() << '\n';
//            }
//        }
//    }

    using schema::best_dim;
    for (unsigned int i = 0; i < data::m; i++)
        logic::evt[logic::n_evt++] = logic::event{data::keys[i][best_dim], i, logic::event::type::KEY};
    for (unsigned int i = 0; i < data::n; i++) {
        logic::evt[logic::n_evt++] = logic::event{data::rules[i][best_dim].first, i, logic::event::type::OPEN};
        logic::evt[logic::n_evt++] = logic::event{data::rules[i][best_dim].second, i, logic::event::type::CLOSE};
    }
    sort(logic::evt.begin(), logic::evt.begin() + logic::n_evt);
    set<unsigned int> open;
    for (unsigned int i = 0; i < logic::n_evt; i++) {
        const logic::event &e = logic::evt[i];
        if (e.type == logic::event::type::OPEN)
            open.insert(e.id);
        if (e.type == logic::event::type::KEY) {
            for (const auto it: open) {
                if (utils::match(data::rules[it], data::keys[e.id])) {
                    data::ans[e.id] = it;
                    break;
                }
            }
        }
        if (e.type == logic::event::type::CLOSE)
            open.erase(e.id);
    }

    for (unsigned int i = 0; i < data::m; i++)
        cout << data::ans[i] << '\n';
    return 0;
}
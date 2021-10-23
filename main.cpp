//#pragma GCC target ("avx2")
#pragma GCC optimization ("O2")
//#pragma GCC optimization ("unroll-loops")

#include<iostream>
#include<vector>
#include<array>
#include<limits>
#include<algorithm>
#include<set>
#include<cmath>
#include<unordered_set>
#include <unordered_map>
#include <bitset>
#include <cassert>

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

    inline unsigned int convert_l(const unsigned int &x) {
        if (x == 707406378)return 0;
        else if (x == 824846890)return 1;
        else if (x == 707865130)return 2;
        else if (x == 707799594)return 0;
        else if (x == 825240106)return 1;
        else if (x == 808462890)return 0;
        else if (x == 707408170)return 4;
        else if (x == 824848682)return 5;
        else if (x == 707801386)return 4;
        else if (x == 707407914)return 0;
        else if (x == 808071210)return 0;
        else if (x == 707866666)return 2;
        else if (x == 707865136)return 2;
        else if (x == 808071217)return 8;
        else if (x == 707801130)return 0;
        else if (x == 808528432)return 2;
        else if (x == 707406385)return 8;
        else if (x == 707799600)return 0;
        else if (x == 808462896)return 0;
        else if (x == 808528426)return 2;
        else if (x == 824848688)return 5;
        else if (x == 824848426)return 1;
        else if (x == 808071472)return 4;
        else if (x == 825307440)return 7;
        else if (x == 808530218)return 6;
        else if (x == 825305648)return 3;
        else if (x == 808464432)return 0;
        else if (x == 808069680)return 0;
        else if (x == 707866922)return 6;
        else if (x == 707801136)return 0;
        else if (x == 825241898)return 5;
        else if (x == 808529968)return 2;
        else if (x == 707801392)return 4;
        else if (x == 707407921)return 8;
        else if (x == 808071466)return 4;
        else if (x == 824846896)return 1;
        else if (x == 808069674)return 0;
        else if (x == 707866928)return 6;
        else if (x == 808464682)return 4;
        else if (x == 825240112)return 1;
        else if (x == 825307184)return 3;
        else if (x == 825241649)return 9;
        else if (x == 707406384)return 0;
        else if (x == 825307441)return 15;
        else if (x == 707866672)return 2;
        else if (x == 707408176)return 4;
        else if (x == 825241904)return 5;
        else if (x == 824848433)return 9;
        else if (x == 824848432)return 1;
        else if (x == 808071216)return 0;
        else if (x == 825307434)return 7;
        else if (x == 825241648)return 1;
        else if (x == 707407920)return 0;
        else if (x == 808528433)return 10;
        else if (x == 707408177)return 12;
        else if (x == 707801137)return 8;
        else if (x == 808529969)return 10;
        else if (x == 707801393)return 12;
        else if (x == 825307185)return 11;
        else if (x == 707866673)return 10;
        else if (x == 808464689)return 12;
        else if (x == 808464433)return 8;
        else if (x == 825305649)return 11;
        else if (x == 825241905)return 13;
        else if (x == 808530225)return 14;
        else if (x == 707866929)return 14;
        else if (x == 825240113)return 9;
        else if (x == 808071473)return 12;
        else if (x == 808529962)return 2;
        else if (x == 824848689)return 13;
        else if (x == 808530224)return 6;
        else if (x == 825307178)return 3;
        else if (x == 808462897)return 8;
        else if (x == 707799601)return 8;
        else if (x == 707865137)return 10;
        else if (x == 808069681)return 8;
        else if (x == 824846897)return 9;
        else if (x == 825305642)return 3;
        else if (x == 808464426)return 0;
        else if (x == 808464688)return 4;
        else if (x == 825241642)return 1;
        return -1;
    }

    inline unsigned int convert_r(const unsigned int &x) {
        if (x == 707406378)return 15;
        else if (x == 824846890)return 15;
        else if (x == 707865130)return 15;
        else if (x == 707799594)return 13;
        else if (x == 825240106)return 13;
        else if (x == 808462890)return 12;
        else if (x == 707408170)return 15;
        else if (x == 824848682)return 15;
        else if (x == 707801386)return 13;
        else if (x == 707407914)return 11;
        else if (x == 808071210)return 10;
        else if (x == 707866666)return 11;
        else if (x == 707865136)return 7;
        else if (x == 808071217)return 10;
        else if (x == 707801130)return 9;
        else if (x == 808528432)return 6;
        else if (x == 707406385)return 15;
        else if (x == 707799600)return 5;
        else if (x == 808462896)return 4;
        else if (x == 808528426)return 14;
        else if (x == 824848688)return 7;
        else if (x == 824848426)return 11;
        else if (x == 808071472)return 6;
        else if (x == 825307440)return 7;
        else if (x == 808530218)return 14;
        else if (x == 825305648)return 7;
        else if (x == 808464432)return 0;
        else if (x == 808069680)return 6;
        else if (x == 707866922)return 15;
        else if (x == 707801136)return 1;
        else if (x == 825241898)return 13;
        else if (x == 808529968)return 2;
        else if (x == 707801392)return 5;
        else if (x == 707407921)return 11;
        else if (x == 808071466)return 14;
        else if (x == 824846896)return 7;
        else if (x == 808069674)return 14;
        else if (x == 707866928)return 7;
        else if (x == 808464682)return 12;
        else if (x == 825240112)return 5;
        else if (x == 825307184)return 3;
        else if (x == 825241649)return 9;
        else if (x == 707406384)return 7;
        else if (x == 825307441)return 15;
        else if (x == 707866672)return 3;
        else if (x == 707408176)return 7;
        else if (x == 825241904)return 5;
        else if (x == 824848433)return 11;
        else if (x == 824848432)return 3;
        else if (x == 808071216)return 2;
        else if (x == 825307434)return 15;
        else if (x == 825241648)return 1;
        else if (x == 707407920)return 3;
        else if (x == 808528433)return 14;
        else if (x == 707408177)return 15;
        else if (x == 707801137)return 9;
        else if (x == 808529969)return 10;
        else if (x == 707801393)return 13;
        else if (x == 825307185)return 11;
        else if (x == 707866673)return 11;
        else if (x == 808464689)return 12;
        else if (x == 808464433)return 8;
        else if (x == 825305649)return 15;
        else if (x == 825241905)return 13;
        else if (x == 808530225)return 14;
        else if (x == 707866929)return 15;
        else if (x == 825240113)return 13;
        else if (x == 808071473)return 14;
        else if (x == 808529962)return 10;
        else if (x == 824848689)return 15;
        else if (x == 808530224)return 6;
        else if (x == 825307178)return 11;
        else if (x == 808462897)return 12;
        else if (x == 707799601)return 13;
        else if (x == 707865137)return 15;
        else if (x == 808069681)return 14;
        else if (x == 824846897)return 15;
        else if (x == 825305642)return 15;
        else if (x == 808464426)return 8;
        else if (x == 808464688)return 4;
        else if (x == 825241642)return 9;
        return -1;
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
                for (unsigned int j = 0; j < data::IP_SIZE; j += 4) {
                    l <<= 4;
                    l |= utils::convert_l(*((unsigned int *) (token.data() + (unsigned char) j)));
                    r <<= 4;
                    r |= utils::convert_r(*((unsigned int *) (token.data() + (unsigned char) j)));
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

//    unordered_map<unsigned int, unsigned int> convertl;
//    unordered_map<unsigned int, unsigned int> convertr;
//
//    const string alf = "01*";
//    for (const char &v1: alf) {
//        for (const char &v2: alf) {
//            for (const char &v3: alf) {
//                for (const char &v4: alf) {
//                    convertl.emplace(v1 + v2 * 256 + v3 * 256 * 256 + v4 * 256 * 256 * 256,
//                                     (v1 == '*' ? 0 : v1 - '0') * 8 + (v2 == '*' ? 0 : v2 - '0') * 4 +
//                                     (v3 == '*' ? 0 : v3 - '0') * 2 + (v4 == '*' ? 0 : v4 - '0'));
//                    convertr.emplace(v1 + v2 * 256 + v3 * 256 * 256 + v4 * 256 * 256 * 256,
//                                     (v1 == '*' ? 1 : v1 - '0') * 8 + (v2 == '*' ? 1 : v2 - '0') * 4 +
//                                     (v3 == '*' ? 1 : v3 - '0') * 2 + (v4 == '*' ? 1 : v4 - '0'));
//                }
//            }
//        }
//    }
//    for (auto cur : convertl){
//        cout << "else if (x == " << cur.first << ")return "<<cur.second << ";\n";
//    }
//    for (auto cur : convertr){
//        cout << "else if (x == " << cur.first << ")return "<<cur.second << ";\n";
//    }
//    const string alf = "01*";
//    for(const char &v1 : alf){
//        for(const char &v2 : alf){
//            for(const char &v3 : alf){
//                for(const char &v4 : alf){
//                    string in;
//                    in += v1;
//                    in += v2;
//                    in += v3;
//                    in += v4;
//                    string out = in;
//                    replace(out.begin(),  out.end(), '*', '1');
//                    string res = bitset<4>(convert_r(*(unsigned int*)in.data())).to_string();
//                    if (out != res)
//                        printf("in: %s, out: %s, convert_r: %s\n", in.data(), out.data(), res.data());
//                }
//            }
//        }
//    }
//    string token = "11000011110111000100110011110111";
//    unsigned int l = 0, r = 0;
//    for (unsigned int j = 0; j < data::IP_SIZE; j += 4) {
//        l <<= 4;
//        l |= utils::convert_l(*((unsigned int *) (token.data() + (unsigned char) j)));
//        r <<= 4;
//        r |= utils::convert_r(*((unsigned int *) (token.data() + (unsigned char) j)));
//    }
//    cout << "l: " << bitset<32>(l) << ", r: " << bitset<32>(r) << endl;
//    return 0;


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
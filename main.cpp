#include<iostream>
#include<vector>
#include<array>

using namespace std;

//#define LITTLE

namespace schema {
    enum dimension_type {
        IP, PORT
    };
    size_t n_ips, n_ports;
    size_t n_dims;

    const size_t MAXD = 5;
    array<dimension_type, MAXD> dims;
}

namespace data {
#ifdef LITTLE
    const size_t IP_SIZE = 5;
#else
    const size_t IP_SIZE = 32;
#endif
    size_t n, m;
    const size_t MAXN = 150000;
    const size_t MAXM = 15000;

    array<array<pair<unsigned int, unsigned int>, schema::MAXD>, MAXN> rules;
    array<array<unsigned int, schema::MAXD>, MAXM> keys;
}

namespace utils {
    bool match(const array<pair<unsigned int, unsigned int>, schema::MAXD> &rule,
               const array<unsigned int, schema::MAXD> &key) {
        for (size_t i = 0; i < schema::n_dims; i++)
            if (key[i] < rule[i].first or key[i] > rule[i].second)
                return false;
        return true;
    }

}

namespace parsing {
    pair<unsigned int, unsigned int> parse_rule_token(const string &token, const schema::dimension_type &type) {
        unsigned int l = 0, r = 0;
        if (type == schema::dimension_type::IP) {
            for (const auto &c: token) {
                l <<= 1, r <<= 1;
                if (c == '*')
                    r |= 1;
                else {
                    l |= (c - '0');
                    r |= (c - '0');
                }
            }
        } else {
            auto div = token.find('-');
            l = (unsigned short) stoi(token.substr(0, div));
            r = (unsigned short) stoi(token.substr(div + 1));
        }
        return {l, r};
    }

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
        for (size_t i = 0; i < schema::n_dims; ++i)
            ranges[i] = parse_rule_token(tokens[i], schema::dims[i]);
        return ranges;
    }

    array<unsigned int, schema::MAXD>
    parse_key(const vector<string> &tokens) {
        array<unsigned int, schema::MAXD> ret;
        for (size_t i = 0; i < schema::n_dims; ++i)
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

                getline(cin, line);
                size_t r = 0;
                while ((r = line.find(' ')) != string::npos) {
                    tokens.push_back(line.substr(0, r));
                    line.erase(0, r + 1);
                }
                tokens.push_back(line);
            }
            schema::n_ips = schema::n_ports = 0;
            for (const auto &token: tokens) {
                if (token.find('-') == string::npos) {
                    schema::dims[schema::n_dims++] = schema::dimension_type::IP;
                    schema::n_ips++;
                } else {
                    schema::dims[schema::n_dims++] = schema::dimension_type::PORT;
                    schema::n_ports++;
                }
            }
            data::rules[0] = parse_rule(tokens);
        }

        for (size_t i = 1; i < data::n; i++) {
            vector<string> tokens(schema::n_dims);
            for (size_t j = 0; j < schema::n_dims; j++)
                cin >> tokens[j];
            data::rules[i] = parse_rule(tokens);
        }

        cin >> data::m;

        for (size_t i = 0; i < data::m; i++) {
            vector<string> tokens(schema::n_dims);
            for (size_t j = 0; j < schema::n_dims; j++)
                cin >> tokens[j];
            data::keys[i] = parse_key(tokens);
        }
    }
}

int main() {
    parsing::parse_input();

    for (int i = 0; i < data::m; i++) {
        int ans = -1;
        for (int j = 0; j < data::n; j++) {
            if (utils::match(data::rules[j], data::keys[i])) {
                ans = j;
                break;
            }
        }
        cout << ans << '\n';
    }

    return 0;
}
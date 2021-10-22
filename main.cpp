#include<iostream>
#include<vector>
#include<array>

using namespace std;

// #define LITTLE

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
    typedef array<char, IP_SIZE> ip;
    typedef unsigned short port;

    size_t n, m;
    const size_t MAXN = 150000;
    const size_t MAXM = 15000;

    array<array<ip, schema::MAXD>, MAXN> ip_rules;
    array<array<pair<port, port>, schema::MAXD>, MAXN> port_rules;

    array<array<ip, schema::MAXD>, MAXM> ip_keys;
    array<array<port, schema::MAXD>, MAXN> port_keys;
}

namespace utils {
    ostream &operator<<(ostream &out, const data::ip &arr) {
        for (size_t i = 0; i < schema::MAXD; i++)
            out << arr[i];
        return out;
    }

    bool match(const data::ip &a, const data::ip &b) {
        for (int i = 0; i < data::IP_SIZE; i++) {
            if (a[i] == '*')
                return true;
            if (a[i] == '0' and b[i] == '1')
                return false;
            if (a[i] == '1' and b[i] == '0')
                return false;
        }
        return true;
    }

    bool match(const pair<data::port, data::port> &a, const data::port &b) {
        return b >= a.first and b <= a.second;
    }
}

namespace parsing {
    pair<array<data::ip, schema::MAXD>, array<pair<data::port, data::port>, schema::MAXD>>
    parse_rule(const vector<string> &tokens) {
        size_t ip_idx = 0, port_idx = 0;
        array<data::ip, schema::MAXD> ips;
        array<pair<data::port, data::port>, schema::MAXD> ports;
        for (size_t i = 0; i < schema::n_dims; ++i) {
            const auto &token = tokens[i];
            if (schema::dims[i] == schema::dimension_type::IP) {
                for (size_t j = 0; j < data::IP_SIZE; j++)
                    ips[ip_idx][j] = token[j];
                ip_idx++;
            } else {
                auto div = token.find('-');
                auto l = (unsigned short) stoi(token.substr(0, div));
                auto r = (unsigned short) stoi(token.substr(div + 1));
                ports[port_idx++] = {l, r};
            }
        }
        return {ips, ports};
    }

    pair<array<data::ip, schema::MAXD>, array<data::port, schema::MAXD>>
    parse_key(const vector<string> &tokens) {
        size_t ip_idx = 0, port_idx = 0;
        array<data::ip, schema::MAXD> ips;
        array<data::port, schema::MAXD> ports;
        for (size_t i = 0; i < schema::n_dims; ++i) {
            const auto &token = tokens[i];
            if (schema::dims[i] == schema::dimension_type::IP) {
                for (size_t j = 0; j < data::IP_SIZE; j++)
                    ips[ip_idx][j] = token[j];
                ip_idx++;
            } else
                ports[port_idx++] = stoi(token);
        }
        return {ips, ports};
    }

    void parse_input() {
        using utils::operator<<;
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
            auto rule = parsing::parse_rule(tokens);
            data::ip_rules[0] = rule.first;
            data::port_rules[0] = rule.second;
        }

        for (size_t i = 1; i < data::n; i++) {
            vector<string> tokens(schema::n_dims);
            for (size_t j = 0; j < schema::n_dims; j++)
                cin >> tokens[j];
            auto rule = parsing::parse_rule(tokens);
            data::ip_rules[i] = rule.first;
            data::port_rules[i] = rule.second;
        }

        cin >> data::m;

        for (size_t i = 0; i < data::m; i++) {
            vector<string> tokens(schema::n_dims);
            for (size_t j = 0; j < schema::n_dims; j++)
                cin >> tokens[j];
            auto key = parsing::parse_key(tokens);
            data::ip_keys[i] = key.first;
            data::port_keys[i] = key.second;
        }
    }
}

int main() {
    parsing::parse_input();

    for (int i = 0; i < data::m; i++) {
        int ans = -1;
        for (int j = 0; j < data::n; j++) {
            bool found = true;
            for (int k = 0; k < schema::n_ips; k++) {
                if (not utils::match(data::ip_rules[j][k], data::ip_keys[i][k])) {
                    found = false;
                    break;
                }
            }
            if (not found)
                continue;
            for (int k = 0; k < schema::n_ports; k++) {
                if (not utils::match(data::port_rules[j][k], data::port_keys[i][k])) {
                    found = false;
                    break;
                }
            }
            if (found) {
                ans = j;
                break;
            }
        }
        cout << ans << '\n';
    }

    return 0;
}
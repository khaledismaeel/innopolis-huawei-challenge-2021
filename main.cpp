#include<iostream>
#include<vector>
#include<array>
#include<fstream>
#include<algorithm>
#include<set>

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

    vector<size_t> answers;
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

    string show_rule(size_t index){
        string s;
        s += string(data::ip_rules[index][0].begin(), data::ip_rules[index][0].end()) + " ";
        s += string(data::ip_rules[index][1].begin(), data::ip_rules[index][1].end()) + " ";
        s += to_string(data::port_rules[index][0].first) + "-" + to_string(data::port_rules[index][0].second) + " ";
        s += to_string(data::port_rules[index][1].first) + "-" + to_string(data::port_rules[index][1].second) + " ";
        s += to_string(data::port_rules[index][2].first) + "-" + to_string(data::port_rules[index][2].second) + " ";
        return s;
    }

    string show_key(size_t index){
        string s;
        s += string(data::ip_keys[index][0].begin(), data::ip_keys[index][0].end()) + " ";
        s += string(data::ip_keys[index][1].begin(), data::ip_keys[index][1].end()) + " ";
        s += to_string(data::port_keys[index][0]) + " ";
        s += to_string(data::port_keys[index][1]) + " ";
        s += to_string(data::port_keys[index][2]) + " ";
        return s;
    }

    int find_key_for_rule(size_t rule_id){
        auto ans_it = find(data::answers.begin(), data::answers.end(), rule_id);
        if(ans_it == data::answers.end()){
            return -1;
        }else{
            return ans_it - data::answers.begin();
        }
    }

    void populate_answer_in_range(int from, int to, vector<string> &ans_rules, vector<pair<int, string>> &ans_keys, int &ans_id){
        for (size_t cur = from; cur < to; ++cur) {
            int ans = utils::find_key_for_rule(cur);
            if (ans != -1){
                ans_rules.push_back(utils::show_rule(cur));
                ans_keys.emplace_back(ans_id, utils::show_key(ans));
                ans_id++;
            }
        }
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

    void parse_output(){
        ifstream fin("correct_output.txt");
        size_t temp;
        while(fin >> temp){
            data::answers.push_back(temp);
        }
    }
}

int main() {
    parsing::parse_input();
    parsing::parse_output();

    vector<size_t> interesting_rules({0, 1, 2, 3, 4, 100, 101, 102, 103, 3440, 3480, 4500, 4550, 13530, 23020, 53272, 58010, 100792, 128750, 130539, 130554});

    vector<string> ans_rules;
    vector<pair<int, string>> ans_keys;
    int ans_id = 0;

    utils::populate_answer_in_range(0, 50, ans_rules, ans_keys, ans_id);
    utils::populate_answer_in_range(100, 150, ans_rules, ans_keys, ans_id);
    utils::populate_answer_in_range(3440, 3480, ans_rules, ans_keys, ans_id);
    utils::populate_answer_in_range(4500, 4600, ans_rules, ans_keys, ans_id);
    utils::populate_answer_in_range(53272, 53350, ans_rules, ans_keys, ans_id);
    utils::populate_answer_in_range(128750, 128800, ans_rules, ans_keys, ans_id);
    utils::populate_answer_in_range(130539, 130600, ans_rules, ans_keys, ans_id);
    utils::populate_answer_in_range(130554, 130700, ans_rules, ans_keys, ans_id);

    cout << "input:\r\n";
    cout << ans_rules.size() << "\r\n";

    for(const auto &cur : ans_rules){
        cout << cur << "\r\n";
    }

    cout << ans_keys.size() << "\r\n";

    for(const auto &cur : ans_keys){
        cout << cur.second << "\r\n";
    }

    cout << "output:\r\n";

    for(const auto &cur : ans_keys){
        cout << cur.first << "\r\n";
    }

    return 0;
}
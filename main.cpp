#include<iostream>
#include<vector>

using namespace std;

enum dimension_type {
    TERNARY, RANGE
};

struct rule {
    vector<string> ternaries;
    vector<pair<unsigned short, unsigned short>> ranges;

    rule() = default;

    rule(const vector<string> &tokens, const vector<dimension_type> &schema) {
        for (int i = 0; i < tokens.size(); ++i) {
            const auto &token = tokens[i];
            if (schema[i] == dimension_type::TERNARY)
                ternaries.push_back(token);
            else {
                auto div = token.find("-");
                auto l = (unsigned short) stoi(token.substr(0, div));
                auto r = (unsigned short) stoi(token.substr(div + 1));
                ranges.emplace_back(l, r);
            }
        }
    }
};

struct key {
    vector<string> ternaries;
    vector<unsigned short> ranges;

    key() = default;

    key(const vector<string> &tokens, const vector<dimension_type> &schema) {
        for (int i = 0; i < tokens.size(); ++i) {
            const auto &token = tokens[i];
            if (schema[i] == dimension_type::TERNARY)
                ternaries.push_back(token);
            else
                ranges.push_back((unsigned short)stoi(token));
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    size_t n; // rule count
    cin >> n;

    vector<rule> rules(n);

    vector<dimension_type> dims;
    {
        vector<string> tokens;
        {
            string line;

            cin.ignore();
            getline(cin, line);
            size_t r = 0;
            while ((r = line.find(" ")) != string::npos) {
                tokens.push_back(line.substr(0, r));
                line.erase(0, r + 1);
            }
            tokens.push_back(line);
        }
        for (const auto &token: tokens) {
            if (token.find("-") == string::npos)
                dims.push_back(dimension_type::TERNARY);
            else
                dims.push_back(dimension_type::RANGE);
        }

        rules[0] = rule(tokens, dims);
    }

    for (int i = 1; i < n; i++) {
        vector<string> tokens(dims.size());
        for (int j = 0; j < dims.size(); j++)
            cin >> tokens[j];
        rules[i] = rule(tokens, dims);
    }

    size_t m; // key count
    cin >> m;


    vector<key> keys(m);
    for (int i = 0; i < m; i++) {
        vector<string> tokens(dims.size());
        for (int j = 0; j < dims.size(); j++)
            cin >> tokens[j];
        keys[i] = key(tokens, dims);
    }



    return 0;
}
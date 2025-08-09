#include <bits/stdc++.h>
using namespace std;

// Convert string in given base to decimal
long long baseToDecimal(const string &value, int base) {
    long long result = 0;
    for (char c : value) {
        int digit;
        if (isdigit(c))
            digit = c - '0';
        else
            digit = tolower(c) - 'a' + 10;
        result = result * base + digit;
    }
    return result;
}

// Lagrange interpolation to find f(0)
long double lagrangeInterpolation(const vector<pair<long long, long long>> &points) {
    long double secret = 0.0;
    int k = points.size();
    for (int i = 0; i < k; i++) {
        long double term = points[i].second; // y_i
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0.0 - points[j].first) / (points[i].first - points[j].first);
            }
        }
        secret += term;
    }
    return secret;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string fileName;
    cout << "Enter JSON file name (with path if needed): ";
    cin >> fileName;

    ifstream file(fileName);
    if (!file) {
        cerr << "Error: Could not open file.\n";
        return 1;
    }

    string jsonContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Remove braces and quotes for simple parsing
    for (char &c : jsonContent) {
        if (c == '{' || c == '}' || c == '"' || c == ',')
            c = ' ';
    }

    stringstream ss(jsonContent);
    string key;
    int n = 0, k = 0;
    vector<pair<long long, long long>> points;

    while (ss >> key) {
        if (key == "n:") {
            ss >> n;
        } else if (key == "k:") {
            ss >> k;
        } else if (isdigit(key[0])) { // x value
            long long x = stoll(key);
            string baseKey, baseVal, valueKey, valueVal;
            ss >> baseKey >> baseVal >> valueKey >> valueVal;
            int base = stoi(baseVal);
            long long y = baseToDecimal(valueVal, base);
            points.push_back({x, y});
        }
    }

    if (points.size() < k) {
        cerr << "Error: Not enough points to interpolate.\n";
        return 1;
    }

    // Use first k points
    vector<pair<long long, long long>> neededPoints(points.begin(), points.begin() + k);

    long double secret = lagrangeInterpolation(neededPoints);
    cout << fixed << setprecision(0) << "Secret (C) = " << secret << "\n";

    return 0;
}

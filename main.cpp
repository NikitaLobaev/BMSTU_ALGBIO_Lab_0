#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

typedef std::pair<std::string, std::string> DNA; //<dna_id, dna_string>

void read_dnas(const std::string&, DNA&, std::vector<DNA>&);

size_t find_lcs_length(const std::string&, const std::string&);

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: lab1 <filename> <count>" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    const size_t count = std::stoul(argv[2]);

    DNA main_dna;
    std::vector<DNA> dnas;

    read_dnas(filename, main_dna, dnas);

    std::cout << "Starting sort... (it may require some time)" << std::endl;

    std::priority_queue<std::pair<size_t, DNA>> pq;
    for (size_t index = std::min(count, dnas.size()); index > 0; index--) {
        pq.emplace(find_lcs_length(main_dna.second, dnas[index - 1].second), dnas[index - 1]);
    }
    for (size_t index = count; index < dnas.size(); index++) {
        pq.emplace(find_lcs_length(main_dna.second, dnas[index].second), dnas[index]);
        pq.pop();
    }

    std::vector<DNA> top_dnas;
    while (!pq.empty()) {
        const DNA dna = pq.top().second;
        top_dnas.push_back(dna);
        pq.pop();
    }
    top_dnas.shrink_to_fit();

    std::cout << std::endl << "<DNA ID>: <DNA>" << std::endl << std::endl;
    std::cout << main_dna.first << ": " << main_dna.second << std::endl << std::endl;
    for (auto it = top_dnas.rbegin(); it != top_dnas.rend(); it++) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    return 0;
}

size_t find_lcs_length(const std::string &x, const std::string &y) {
    if (x.empty() || y.empty()) {
        return 0;
    }

    const size_t x_length = x.length();
    const size_t y_length = y.length();

    std::vector<std::vector<size_t>> dp(x_length + 1, std::vector<size_t>(y_length + 1));

    for (size_t i = 1; i <= x_length; i++) {
        for (size_t j = 1; j <= y_length; j++) {
            if (x[i - 1] == y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else if (dp[i - 1][j] >= dp[i][j - 1]) {
                dp[i][j] = dp[i - 1][j];
            } else {
                dp[i][j] = dp[i][j - 1];
            }
        }
    }

    return dp[x_length][y_length];
}

void read_dnas(const std::string &filename, DNA &main_dna, std::vector<DNA> &dnas) {
    std::ifstream ifstream(filename);
    std::string cur_line, cur_dna_buf;
    while (std::getline(ifstream, cur_line)) {
        if (cur_line.empty()) {
            continue;
        }

        if (cur_line[0] == '>') {
            if (!cur_dna_buf.empty()) {
                if (main_dna.first.empty()) {
                    main_dna = {dnas.back().first, cur_dna_buf};
                    dnas.pop_back();
                } else {
                    dnas.back().second = cur_dna_buf;
                }
                cur_dna_buf.clear();
            }

            const size_t id_start_index = cur_line.find('|');
            const size_t id_length = cur_line.substr(id_start_index + 1).find('|');
            const std::string id = cur_line.substr(id_start_index + 1, id_length);
            dnas.emplace_back(id, "");
        } else {
            cur_dna_buf += cur_line;
        }
    }

    if (!cur_dna_buf.empty()) {
        if (main_dna.first.empty()) {
            main_dna = {dnas.back().first, cur_dna_buf};
            dnas.pop_back();
        } else {
            dnas.back().second = cur_dna_buf;
        }
    }
}

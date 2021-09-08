#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

typedef std::pair<std::string, std::string> DNA; //<dna_id, dna_string>

void read_dnas(const std::string&, DNA&, std::vector<DNA>&);

std::string find_lcs(const std::string&, const std::string&);

bool compare_dnas(const DNA&, const DNA&, const DNA&);

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: lab1 <filename>" << std::endl;
        return 1;
    }

    DNA main_dna;
    std::vector<DNA> dnas;

    read_dnas(argv[1], main_dna, dnas);

    std::cout << "Starting sort... (it may require some minutes)" << std::endl;

    std::sort(dnas.begin(), dnas.end(), [main_dna](const DNA &dna1, const DNA &dna2) {
        return compare_dnas(main_dna, dna1, dna2);
    });

    std::cout << std::endl << "<DNA ID>: <DNA>" << std::endl << std::endl;
    std::cout << main_dna.first << ": " << main_dna.second << std::endl << std::endl;
    for (const DNA &dna : dnas) {
        std::cout << dna.first << ": " << dna.second << std::endl;
    }

    return 0;
}

enum Direction {
    UP, LEFT, UP_LEFT
};

std::string count_lcs(const std::string&, const std::vector<std::vector<Direction>>&, size_t, size_t);

std::string find_lcs(const std::string &x, const std::string &y) {
    if (x.empty() || y.empty()) {
        return "";
    }

    const size_t x_length = x.length();
    const size_t y_length = y.length();

    std::vector<std::vector<Direction>> directions(x_length + 1, std::vector<Direction>(y_length + 1));
    std::vector<std::vector<size_t>> dp(x_length + 1, std::vector<size_t>(y_length + 1));

    for (size_t i = 1; i <= x_length; i++) {
        for (size_t j = 1; j <= y_length; j++) {
            if (x[i - 1] == y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                directions[i][j] = UP_LEFT;
            } else if (dp[i - 1][j] >= dp[i][j - 1]) {
                dp[i][j] = dp[i - 1][j];
                directions[i][j] = UP;
            } else {
                dp[i][j] = dp[i][j - 1];
                directions[i][j] = LEFT;
            }
        }
    }

    return count_lcs(x, directions, x_length, y_length);
}

std::string count_lcs(const std::string &string, const std::vector<std::vector<Direction>> &directions,
                      size_t i, size_t j) {
    if (i == 0 || j == 0) {
        return "";
    }

    if (directions[i][j] == UP_LEFT) {
        return count_lcs(string, directions, i - 1, j - 1) + string[i - 1];
    } else if (directions[i][j] == UP) {
        return count_lcs(string, directions, i - 1, j);
    } else {
        return count_lcs(string, directions, i, j - 1);
    }
}

void read_dnas(const std::string &filename, DNA &main_dna, std::vector<DNA> &dnas) {
    std::ifstream ifstream(filename);
    for (std::string cur_line, cur_dna_buf; std::getline(ifstream, cur_line);) {
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
}

bool compare_dnas(const DNA &main_dna, const DNA &dna1, const DNA &dna2) {
    const std::string max1 = find_lcs(main_dna.second, dna1.second);
    const std::string max2 = find_lcs(main_dna.second, dna2.second);
    return max1.length() > max2.length(); //greater comparator
}

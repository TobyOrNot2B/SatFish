#include <iostream>
#include <sstream>
#include <satFishLib/CNF.h>
#include <satFishLib/DPLL.h>
#include <filesystem>
#include <set>
#include <algorithm>
#include <fstream>

bool compareFileEntries(const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
    return a.path().filename().string() < b.path().filename().string();
}

int main(int argc, char **argv)
{
    string path;

    if (argc <= 2) {
        if (argc == 1) {
            std::cout << "No input file specified. please provide a file path to a cnf" << endl;
            std::cout << "Input file: ";
            std::cin >> path;
        }
        else {
            path = argv[1];
            std::cout << "Input file: " << path << endl;
        }
        CNF cnf(path);
        vector<int> solution = solve(cnf);
        cout << "size: " << solution.size() << endl;

        //put solution in a sorted set
        std::set<int> sorted_solution(solution.begin(), solution.end());

        //print solution
        cout << "solution: ";
        for (int i : sorted_solution) {
            cout << i << " ";
        }
        cout << endl;
    }
    else if (argc == 3) {
        string path = argv[1];
        if (path == "--benchmark") {
            path = argv[2];
        }
        std::cout << "Benchmark Dir: " << path << endl;

        //benchmark results path
        string resultPath = path + "\\benchmarkresults.csv";
        std::ofstream outputfile(resultPath);
        if (!outputfile.is_open()) {
            std::cerr << "couldnt open output file" << std::endl;
        }


        vector<std::filesystem::directory_entry> files;
        std::copy(std::filesystem::directory_iterator(path), std::filesystem::directory_iterator(), std::back_inserter(files));
        std::sort(files.begin(), files.end(), compareFileEntries);

        for (auto entry : files) {
            //if the file extension is not cnf, skip it
            if (entry.path().extension() != ".cnf") {
                continue;
            }

            CNF cnf(entry.path().string());
            //start a timer
            auto start = std::chrono::high_resolution_clock::now();
            vector<int> solution = solve(cnf);
            auto end = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            std::stringstream outputstream;
            outputstream << entry.path().filename() << ", "
                << cnf.getLiteralCount() << ", "
                << cnf.size() << ", "
                << duration << ", " << std::endl;

            outputfile << outputstream.str();
            cout << outputstream.str();
        }
        std::cout << "Results file: " << argv[2] << endl;
    }
    else {
        std::cout << "Too many arguments provided" << endl;
        std::cout << "Usage: " << argv[0] << " <input file>" << endl;
    }

    return 0;
}


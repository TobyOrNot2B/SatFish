#include <iostream>
#include "Clause.h"
#include "CNF.h"
#include "DPLL.h"
#include <filesystem>
#include <set>

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
        unordered_set<int> solution = solve(cnf);
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


        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            //if the file extension is not cnf, skip it
            if (entry.path().extension() != ".cnf") {
				continue;
			}

            CNF cnf(entry.path().string());
            //start a timer
            auto start = std::chrono::high_resolution_clock::now();
            unordered_set<int> solution = solve(cnf);
            auto end = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            outputfile << entry.path().filename() << ", "
                << cnf.literalCount() << ", "
                << cnf.size() << ", "
                << duration << ", " << std::endl;
        }
        return 0;

		std::cout << "Results file: " << argv[2] << endl;
    }
    else {
        std::cout << "Too many arguments provided" << endl;
        std::cout << "Usage: " << argv[0] << " <input file>" << endl;
	}

    return 0;
}

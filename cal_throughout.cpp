#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <limits>

void calculateThroughput(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inFile(inputFileName);
    std::ofstream outFile(outputFileName);

    // 检查文件是否成功打开
    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return;
    }

    std::string line;
    std::map<int, double> throughputMap;
    getline(inFile, line); // 跳过第一行（标题行）

    // 读取每一行
    while (getline(inFile, line)) {
        std::stringstream ss(line);
        std::string cellIdStr, rxBytesStr;
        int cellId;
        double rxBytes;

        // 获取第三列（CellId）和第十列（RxBytes）
        for (int i = 0; i < 10; ++i) {
            if (i == 2) {
                getline(ss, cellIdStr, '\t');
            } else if (i == 9) {
                getline(ss, rxBytesStr, '\t');
            } else {
                ss.ignore(std::numeric_limits<std::streamsize>::max(), '\t');
            }
        }

        // 转换字符串为整数和浮点数
        std::stringstream(cellIdStr) >> cellId;
        std::stringstream(rxBytesStr) >> rxBytes;

        // 计算吞吐量并累加
        throughputMap[cellId] += (rxBytes * 8 / 1000 / 50);
    }

    // 输出结果到文件
    outFile << "CellId,Throughput\n";
    for (const auto& pair : throughputMap) {
        outFile << pair.first << "," << pair.second << std::endl;
    }

    inFile.close();
    outFile.close();
}

int main() {
    std::string inputFileName = "dlrdatastats.txt";
    std::string outputFileName = "tun.txt";
    calculateThroughput(inputFileName, outputFileName);
    
    inputFileName = "ulrdatastats.txt";
    outputFileName = "tu.txt";
    calculateThroughput(inputFileName, outputFileName);

    return 0;
}

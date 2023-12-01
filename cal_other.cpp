#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>

struct Stats {
    double totalRsrp = 0.0;
    double totalSinr = 0.0;
    int count = 0;
};

// 将 SINR 从线性刻度转换为分贝
double convertToDb(double sinr) {
    return 10 * log10(sinr);
}

int main() {
    std::ifstream inFile("dlrspsinr.txt");
    std::ofstream outFile("cellId_avg_stats.csv");

    if (!inFile.is_open()) {
        std::cerr << "无法打开输入文件" << std::endl;
        return 1;
    }

    if (!outFile.is_open()) {
        std::cerr << "无法打开输出文件" << std::endl;
        return 1;
    }

    std::string line;
    std::map<int, Stats> dataMap;

    // 跳过第一行标题
    std::getline(inFile, line);

    // 读取数据
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        int cellId;
        double rsrp, sinr;

        // 读取列：time, cellId, IMSI, RNTI, rsrp, sinr, ComponentCarrierId
        ss >> line >> cellId >> line >> line >> rsrp >> sinr >> line;

        // 累加 RSRP 和 SINR，以及计数
        dataMap[cellId].totalRsrp += rsrp;
        dataMap[cellId].totalSinr += sinr;
        dataMap[cellId].count++;
    }

    // 计算平均值并输出到文件
    outFile << "CellId,Average RSRP,Average SINR (dB)\n";
    for (const auto& pair : dataMap) {
        double avgRsrp = pair.second.totalRsrp / pair.second.count;
        double avgSinrDb = convertToDb(pair.second.totalSinr / pair.second.count);
        outFile << pair.first << "," << avgRsrp << "," << avgSinrDb << std::endl;
    }

    inFile.close();
    outFile.close();

    return 0;
}


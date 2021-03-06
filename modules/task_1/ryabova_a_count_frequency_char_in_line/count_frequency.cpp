// Copyright 2019 Ryabova Alyona

#include <mpi.h>
#include <cstring>
#include <string>
#include <ctime>
#include <random>
#include "../../../modules/task_1/ryabova_a_count_frequency_char_in_line/count_frequency.h"


std::string getRandomString(int size) {
    static const char alphanum[] =
        "0123456789"
        "QWERTYUIOPASDFGHJKLZXCVBNM"
        "qwertyuiopasdfghjklzxcvbnm";
    int alphanumLen = std::strlen(alphanum);
    std::string str(size, 0);
    for (int i = 0; i < size; i++) {
        str[i] = alphanum[rand() % (alphanumLen - 1)];
    }
    return str;
}


int getCount(std::string str, char c) {
    int count = 0;
    size_t len = str.length();
    for (size_t i = 0; i < len; i++) {
        if (str[i] == c)
            count++;
    }
    return count;
}

int getCountCharInStr(std::string str, char ch) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int len = static_cast<int>(str.length());
    if (ProcNum < 2) {
        int count = getCount(str, ch);
        return count;
    }
    int step = len / ProcNum;
    int countProc = len % ProcNum;

    MPI_Status status;
    int localCounter = 0;
    int globalCounter = 0;
    std::string localStr("", step);

    MPI_Barrier(MPI_COMM_WORLD);

    if (ProcRank == 0) {
        int counter;
        for (int proc = 0; proc < ProcNum - countProc; proc++) {
            localStr = "";
            counter = 0;
            counter = step * proc;
            for (int i = 0; i < step; i++)
                localStr += str[i + counter];
            if (proc == 0) {
                globalCounter += getCount(localStr, ch);
            } else {
                MPI_Send(&localStr[0], step, MPI_CHAR, proc, 0, MPI_COMM_WORLD);
            }
        }
        for (int proc = ProcNum - countProc; proc < ProcNum; proc++) {
            int counter = len + (step + 1) * (proc - ProcNum);
            localStr = "";
            for (int i = 0; i < step + 1; i++)
                localStr += str[i + counter];
            if (proc == 0) {
                globalCounter += getCount(localStr, ch);
            } else {
                MPI_Send(&localStr[0], (step + 1), MPI_CHAR, proc, 2, MPI_COMM_WORLD);
            }
        }
        for (int proc = 1; proc < ProcNum; proc++) {
            MPI_Recv(&localCounter, 1, MPI_INT, MPI_ANY_SOURCE,
            1, MPI_COMM_WORLD, &status);
            globalCounter += localCounter;
        }
    } else {
        if (ProcRank < ProcNum - countProc)
            MPI_Recv(&localStr[0], step, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        if (ProcRank >= ProcNum - countProc && ProcRank < ProcNum) {
            localStr.resize(step+1);
            MPI_Recv(&localStr[0], step + 1, MPI_CHAR, 0, 2, MPI_COMM_WORLD, &status);
        }
        localCounter = getCount(localStr, ch);
        MPI_Send(&localCounter, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
    return globalCounter;
}

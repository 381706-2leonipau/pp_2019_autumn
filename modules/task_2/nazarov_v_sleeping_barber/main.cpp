// Copyright 2019 Nazarov Vladislav
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./sleeping_barber.h"

TEST(Sleeping_barber, Test_Serve) {
    int rank;
    int not_enough_chair = 0;
    int served = sleeping_barber(5, 25, &not_enough_chair);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int flag;  // clearing buffer
    int mes;
    MPI_Status status;
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    while (flag) {
        MPI_Recv(&mes, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        ASSERT_EQ(served, 25);
    }
}

TEST(Sleeping_barber, Test_No_Chairs) {
    int rank;
    int not_enough_chair = 0;
    int served = sleeping_barber(0, 25, &not_enough_chair);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int flag;  // clearing buffer
    int mes;
    MPI_Status status;
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    while (flag) {
        MPI_Recv(&mes, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        ASSERT_EQ(served, 25);
    }
}

TEST(Sleeping_barber, Test_Not_Enough_Chairs) {
    int rank;
    int not_enough_chair = 0;
    sleeping_barber(0, 15, &not_enough_chair);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int flag;  // clearing buffer
    int mes;
    MPI_Status status;
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    while (flag) {
        MPI_Recv(&mes, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        ASSERT_GT(not_enough_chair, 0);
    }
}

TEST(Sleeping_barber, Test_Nobody_To_Serve) {
    int rank;
    int not_enough_chair = 0;
    int served = sleeping_barber(5, 0, &not_enough_chair);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int flag;  // clearing buffer
    int mes;
    MPI_Status status;
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    while (flag) {
        MPI_Recv(&mes, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        ASSERT_EQ(served, 0);
    }
}

TEST(Sleeping_barber, Test_Negative_Value) {
    int not_enough_chair = 0;
    ASSERT_ANY_THROW(sleeping_barber(5, -10, &not_enough_chair));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}

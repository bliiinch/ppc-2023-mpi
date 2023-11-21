// Copyright 2023 Lysanova Julia
#include <utility>
#include <iostream>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_2/lysanova_j_rdrs_wrtrs/rdrs_wrtrs.h"

void rdrs_wrtrs_boost() {
    boost::mpi::communicator world;
    int shared_data = 0;
    int role;

    if (world.rank() == 0) {
        for (int i = 0; i < world.size() - 1; i++) {
            auto status = world.recv(MPI_ANY_SOURCE, 0, role);
            std::cout << status.source() << " with role = " << role << " came " <<std::endl;
            if (role == 1) {
                world.recv(status.source(), 1, shared_data);
                std::cout << status.source() <<  " put " << shared_data << std::endl;
            } else {
                world.send(status.source(), 1, shared_data);
            }
        }
        std::cout << "final shared_data = " << shared_data << std::endl;
    } else {
        role = world.rank() % 2;
        world.send(0, 0, role);
        if (role == 1) {
            shared_data = world.rank() * 10;
            world.send(0, 1, shared_data);
        } else {
            world.recv(0, 1, shared_data);
            std::cout << world.rank() << " read " << shared_data << std::endl;
        }
    }
    world.barrier();
}
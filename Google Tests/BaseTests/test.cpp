#include <iostream>
#include <gtest/gtest.h>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>
#include <pthread.h>

#include "../../L2.h"
#include "../../L3.h"
#include "../../L4_TCP.h"
#include "../../NIC.h"
#include "../../L2_ARP.h"
#include "../../NIC_Cable.h"
#include "../../L0_buffer.h"
#include "pch.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}
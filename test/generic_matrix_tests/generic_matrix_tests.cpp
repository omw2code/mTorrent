#include <memory>
#include <gtest/gtest.h>
#include <GenericMatrix.hpp>

/****************** Constructor Tests ********************/
TEST(GenericMatrixIntegralTest, DefaultConstructorIntTest)
{
    auto matrix = std::make_unique<GenericMatrix<int>>();

    ASSERT_NE(matrix, nullptr);
}

TEST(GenericMatrixIntegralTest, DefaultConstructorLongTest)
{
    auto long_matrix = std::make_unique<GenericMatrix<long>>();

    ASSERT_NE(long_matrix, nullptr);
}

TEST(GenericMatrixIntegrlTest, DefaultConstructorShortTest)
{
    auto short_matrix = std::make_unique<GenericMatrix<short>>();

    ASSERT_NE(short_matrix, nullptr);
}

TEST(GenericMatrixFloatingTest, DefaultConstructorFloatTest)
{
    auto float_matrix = std::make_unique<GenericMatrix<float>>();

    ASSERT_NE(float_matrix, nullptr);
}

TEST(GenericMatrixFloatingTest, DefaultConstructorDoubleTest)
{
    auto double_matrix = std::make_unique<GenericMatrix<double>>();

    ASSERT_NE(double_matrix, nullptr);
}

/******************* Initializer List Tests ********************/
TEST(GenericMatrixInitListTest, EmptyInitializerListTest)
{
    GenericMatrix<int> matrix{};

    ASSERT_TRUE(matrix.empty());
    ASSERT_EQ(matrix.getRows(), 0);
    ASSERT_EQ(matrix.getCols(), 0);
}

TEST(GenericMatrixInitListTest, SingleInitializerListTest)
{
    GenericMatrix<int> matrix{{}};
    ASSERT_FALSE(matrix.empty());
    ASSERT_EQ(matrix.getRows(), 1);
    ASSERT_EQ(matrix.getCols(), 0);
};

TEST(GenericMatrixInitListTest, PopulatedInitializerListTest)
{
    GenericMatrix<int> matrix{
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}};

    ASSERT_FALSE(matrix.empty());
    ASSERT_EQ(matrix.getRows(), 3);
    ASSERT_EQ(matrix.getCols(), 4);
}

TEST(GenericMatrixInitListTest, UnequalColsTest)
{
    EXPECT_THROW((
        GenericMatrix<int> { 
            {1, 2, 3, 4}, 
            {5, 6, 7},
            {8, 9, 10 ,11}}),
        std::runtime_error);

}

"""
Feel free to add more test classes and/or tests that are not provided by the skeleton code!
Make sure you follow these naming conventions: https://docs.pytest.org/en/reorganize-docs/goodpractices.html#test-discovery
for your new tests/classes/python files or else they might be skipped.
"""
from utils import *

"""
For each operation, you should write tests to test correctness on matrices of different sizes.
Hint: use rand_dp_nc_matrix to generate dumbpy and numc matrices with the same data and use
      cmp_dp_nc_matrix to compare the results
"""
class TestAddCorrectness:
    def test_small_add(self):
        dp1, nc1 = rand_dp_nc_matrix(2, 3, rand=True)
        dp2, nc2 = rand_dp_nc_matrix(2, 3, rand=True)
        dpr = dp1 + dp2
        ncr = nc1 + nc2
        assert(cmp_dp_nc_matrix(dpr, ncr))

    def test_medium_add(self):
        dp1, nc1 = rand_dp_nc_matrix(25, 25, rand=True)
        dp2, nc2 = rand_dp_nc_matrix(25, 25, rand=True)
        dpr = dp1 + dp2
        ncr = nc1 + nc2
        assert(cmp_dp_nc_matrix(dpr, ncr))

    def test_large_add(self):
        dp1, nc1 = rand_dp_nc_matrix(100, 100, rand=True)
        dp2, nc2 = rand_dp_nc_matrix(100, 100, rand=True)
        dpr = dp1 + dp2
        ncr = nc1 + nc2
        assert(cmp_dp_nc_matrix(dpr, ncr))

class TestSubCorrectness:
    def test_small_sub(self):
        dp1, nc1 = rand_dp_nc_matrix(2, 3, rand=True)
        dp2, nc2 = rand_dp_nc_matrix(2, 3, rand=True)
        dpr = dp1 - dp2
        ncr = nc1 - nc2
        assert(cmp_dp_nc_matrix(dpr, ncr))

    def test_medium_sub(self):
        dp1, nc1 = rand_dp_nc_matrix(25, 25, rand=True)
        dp2, nc2 = rand_dp_nc_matrix(25, 25, rand=True)
        dpr = dp1 - dp2
        ncr = nc1 - nc2
        assert(cmp_dp_nc_matrix(dpr, ncr))

    def test_large_sub(self):
        dp1, nc1 = rand_dp_nc_matrix(100, 100, rand=True)
        dp2, nc2 = rand_dp_nc_matrix(100, 100, rand=True)
        dpr = dp1 - dp2
        ncr = nc1 - nc2
        assert(cmp_dp_nc_matrix(dpr, ncr))

class TestAbsCorrectness:
    def test_small_abs(self):
        # TODO: YOUR CODE HERE
        pass

    def test_medium_abs(self):
        # TODO: YOUR CODE HERE
        pass

    def test_large_abs(self):
        # TODO: YOUR CODE HERE
        pass

class TestNegCorrectness:
    def test_small_neg(self):
        # TODO: YOUR CODE HERE
        pass

    def test_medium_neg(self):
        # TODO: YOUR CODE HERE
        pass

    def test_large_neg(self):
        # TODO: YOUR CODE HERE
        pass

class TestMulCorrectness:
    def test_small_mul(self):
        dp1, nc1 = rand_dp_nc_matrix(2, 3, rand=True)
        dp2, nc2 = rand_dp_nc_matrix(3, 4, rand=True)
        dpr = dp1 * dp2
        ncr = nc1 * nc2
        assert(cmp_dp_nc_matrix(dpr, ncr))

    def test_medium_mul(self):
        dp1, nc1 = rand_dp_nc_matrix(20, 30, rand=True)
        dp2, nc2 = rand_dp_nc_matrix(30, 40, rand=True)
        dpr = dp1 * dp2
        ncr = nc1 * nc2
        assert(cmp_dp_nc_matrix(dpr, ncr))

    def test_large_mul(self):
        # TODO: YOUR CODE HERE
        pass

class TestPowCorrectness:
    def test_small_pow(self):
        n = 4
        dp1, nc1 = rand_dp_nc_matrix(2, 2, rand=True)
        dpr = dp1 ** 4
        ncr = nc1 ** 4
        assert(cmp_dp_nc_matrix(dpr, ncr))

    def test_medium_pow(self):
        # TODO: YOUR CODE HERE
        pass

    def test_large_pow(self):
        # TODO: YOUR CODE HERE
        pass

class TestGetCorrectness:
    def test_get(self):
        # TODO: YOUR CODE HERE
        pass

class TestSetCorrectness:
    def test_set(self):
        # TODO: YOUR CODE HERE
        pass

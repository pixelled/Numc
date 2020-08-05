"""
Feel free to add more test classes and/or tests that are not provided by the skeleton code!
Make sure you follow these naming conventions: https://docs.pytest.org/en/reorganize-docs/goodpractices.html#test-discovery
for your new tests/classes/python files or else they might be skipped.
"""
from utils import *
import time

"""
- We will test you on your performance on add, sub, abs, neg, mul, and pow, so make sure to test these
yourself! We will also test your implementation on matrices on different sizes. It is normal if
your smaller matrices are about the same speed as the naive implementation or even slower.
- Use time.time(), NOT time.perf_counter() to time your program!
- DO NOT count the time for initializing matrices into your performance time. Only count the
time the part where operations are carried out.
- Please also check for correctness while testing for performance!
- We provide the structure for test_small_add. All other tests should have similar structures
"""
class TestAddPerformance:
    def test_small_add(self):
        # Initialize matrices using rand_dp_nc_matrix
        # TODO: YOUR CODE HERE

        nc_start = time.time()
        # Carry out numc matrix operations
        # TODO: YOUR CODE HERE
        nc_end = time.time()

        dp_start = time.time()
        # Carry out dumbpy matrix operations
        # TODO: YOUR CODE HERE
        dp_end = time.time()

        # Check for correctness using cmp_dp_nc_matrix and calculate speedup
        # TODO: YOUR CODE HERE
        pass

    def test_medium_add(self):
        dp1, nc1 = rand_dp_nc_matrix(8000, 12000, rand=True)
        dp2, nc2 = rand_dp_nc_matrix(8000, 12000, rand=True)
        dp_start = time.time()
        dpr = dp1 + dp2
        dp_end = time.time()
        nc_start = time.time()
        ncr = nc1 + nc2
        nc_end = time.time()
        speedup = (dp_end - dp_start) / (nc_end - nc_start)
        print("\nAdd Speedup: {}".format(speedup))

    def test_large_add(self):
        # TODO: YOUR CODE HERE
        pass

class TestSubPerformance:
    def test_small_sub(self):
        # TODO: YOUR CODE HERE
        pass

    def test_medium_sub(self):
        dp1, nc1 = rand_dp_nc_matrix(8000, 12000, rand=True)
        dp2, nc2 = rand_dp_nc_matrix(8000, 12000, rand=True)
        dp_start = time.time()
        dpr = dp1 - dp2
        dp_end = time.time()
        nc_start = time.time()
        ncr = nc1 - nc2
        nc_end = time.time()
        speedup = (dp_end - dp_start) / (nc_end - nc_start)
        print("\nSub Speedup: {}".format(speedup))

    def test_large_sub(self):
        # TODO: YOUR CODE HERE
        pass

class TestAbsPerformance:
    def test_small_abs(self):
        # TODO: YOUR CODE HERE
        pass

    def test_medium_abs(self):
        dp1, nc1 = rand_dp_nc_matrix(8000, 12000, rand=True)
        dp_start = time.time()
        dpr = abs(dp1)
        dp_end = time.time()
        nc_start = time.time()
        ncr = abs(nc1)
        nc_end = time.time()
        speedup = (dp_end - dp_start) / (nc_end - nc_start)
        print("\nAbs Speedup: {}".format(speedup))

    def test_large_abs(self):
        # TODO: YOUR CODE HERE
        pass

class TestNegPerformance:
    def test_small_neg(self):
        # TODO: YOUR CODE HERE
        pass

    def test_medium_neg(self):
        dp1, nc1 = rand_dp_nc_matrix(8000, 12000, rand=True)
        dp_start = time.time()
        dpr = -dp1
        dp_end = time.time()
        nc_start = time.time()
        ncr = -nc1
        nc_end = time.time()
        speedup = (dp_end - dp_start) / (nc_end - nc_start)
        print("\nNeg Speedup: {}".format(speedup))

    def test_large_neg(self):
        # TODO: YOUR CODE HERE
        pass

class TestMulPerformance:
    def test_small_mul(self):
        # TODO: YOUR CODE HERE
        pass

    def test_medium_mul(self):
        # TODO: YOUR CODE HERE
        pass

    def test_large_mul(self):
        dp1, nc1 = rand_dp_nc_matrix(2000, 3000, rand=True)
        dp2, nc2 = rand_dp_nc_matrix(3000, 4000, rand=True)
        dp_start = time.time()
        dpr = dp1 * dp2
        dp_end = time.time()
        nc_start = time.time()
        ncr = nc1 * nc2
        nc_end = time.time()
        speedup = (dp_end - dp_start) / (nc_end - nc_start)
        print("\nMul Speedup: {}".format(speedup))

class TestPowPerformance:
    def test_small_pow(self):
        # TODO: YOUR CODE HERE
        pass

    def test_medium_pow(self):
        dp, nc = rand_dp_nc_matrix(53,53, rand=True)
        dp_start = time.time()
        dpr = dp ** 89
        dp_end = time.time()
        nc_start = time.time()
        ncr = nc ** 89
        nc_end = time.time()
        speedup = (dp_end - dp_start) / (nc_end - nc_start)
        print("\nPow Speedup: {}".format(speedup))

    def test_large_pow(self):
        # TODO: YOUR CODE HERE
        pass

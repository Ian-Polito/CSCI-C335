unsigned int mult_by_7_c(unsigned int x) {
	int result = x << 3;
	result -= x;
	return result;
}

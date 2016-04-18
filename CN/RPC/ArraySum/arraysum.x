struct args {
	int size;
	int arr[128];
};

program ARRAYSUM_CALC {
    version ARRAYSUM_CALC_1 {
        int ARRAYSUM(args) = 1;
    } = 1;
} = 0x1fffffff;

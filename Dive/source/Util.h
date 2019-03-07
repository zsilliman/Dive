#pragma once

class Util {

public:

	/** Same as the normal '%' function except it does not return negatives */
	static int mod(int a, int n) {
		return ((a % n) + n) % n;
	}

};

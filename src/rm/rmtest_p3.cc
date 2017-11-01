
#include "rm_test_util.h"

RC TEST_RM_PRIVATE_3(const string &tableName) {
	// Functions tested
	// 1. Simple Scan
	cerr << endl << "***** In RM Test Case Private 3 *****" << endl;

	RID rid;
	int numTuples = 100000;
	void *returnedData = malloc(300);
	set<int> user_ids;

	// Read UserIds that was created in the private test 1
	readUserIdsFromDisk(user_ids, numTuples);

	// Set up the iterator
	RM_ScanIterator rmsi;
	string attr = "userid";
	vector<string> attributes;
	attributes.push_back(attr);
	attributes.push_back("message_text");
	RC rc = rm->scan(tableName, "", NO_OP, NULL, attributes, rmsi);
	if (rc != success) {
		cerr << "***** RM Test Case Private 3 failed. *****" << endl << endl;
		return -1;
	}

	int userid = 0;
	int count = 0;
	while (rmsi.getNextTuple(rid, returnedData) != RM_EOF) {
		count++;
		userid = *(int *) ((char *) returnedData + 1);
//		if (rid.pageNum == 1449 && rid.slotNum == 21) {
//			cout << "User ID: " << userid;
//			int size = *(int*) ((char*) returnedData + 5);
//			string check;
//			check.assign((char*) returnedData + 9, size);
//			cout << " Check: " << check << endl;
//		}
		if (user_ids.find(userid) == user_ids.end()) {
			cerr << "***** RM Test Case Private 3 failed. *****" << endl << endl;
			rmsi.close();
			free(returnedData);
			return -1;
		}
	}
	cout << "Total is: " << count << endl;
	rmsi.close();
	free(returnedData);

	cerr << "***** RM Test Case Private 3 finished. The result will be examined. *****" << endl << endl;
	return 0;
}

int main() {
	RC rcmain = TEST_RM_PRIVATE_3("tbl_private_1");
	return rcmain;
}

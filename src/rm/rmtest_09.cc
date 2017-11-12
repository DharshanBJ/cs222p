#include "rm_test_util.h"

RC TEST_RM_09(const string &tableName, vector<RID> &rids, vector<int> &sizes)
{
    // Functions Tested for large tables:
    // 1. read tuple
    cout << "***** In RM Test case 9 *****" << endl;

    int size = 0;
    int numTuples = 200;
    void *tuple = malloc(400);
    void *returnedData = malloc(400);
    
    // read the saved rids and the sizes of records
    readRIDsFromDisk(rids, numTuples);
    readSizesFromDisk(sizes, numTuples);

    // GetAttributes
    vector<Attribute> attrs;
    RC rc = rm->getAttributes(tableName, attrs);
    assert(rc == success && "RelationManager::getAttributes() should not fail.");

    int nullAttributesIndicatorActualSize = getActualByteForNullsIndicator(attrs.size());
    unsigned char *nullsIndicator = (unsigned char *) malloc(nullAttributesIndicatorActualSize);
	memset(nullsIndicator, 0, nullAttributesIndicatorActualSize);

    for(int i = 0; i < numTuples; i++)
    {
        memset(tuple, 0, 400);
        memset(returnedData, 0, 400);
        rc = rm->readTuple(tableName, rids[i], returnedData);
        assert(rc == success && "RelationManager::readTuple() should not fail.");

        size = 0;
        prepareLargeTuple(attrs.size(), nullsIndicator, i, tuple, &size);
        if(memcmp(returnedData, tuple, sizes[i]) != 0)
        {
            cout << "***** [FAIL] Test Case 9 Failed *****" << endl << endl;
            free(tuple);
            free(returnedData);
            free(nullsIndicator);
            return -1;
        }
    }

    free(tuple);
    free(returnedData);
    free(nullsIndicator);

    cout << "***** Test Case 9 Finished. The result will be examined. *****" << endl << endl;
    
    return success;
}

int main()
{
    vector<RID> rids;
    vector<int> sizes;

	// Read Tuple
    RC rcmain = TEST_RM_09("tbl_employee4", rids, sizes);

    return rcmain;
}

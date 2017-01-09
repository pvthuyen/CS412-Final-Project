#include "configurations.h"
#include "initialize.h"
#include "test.h"
#include "server.h"

#include "app/appdata.h"
#include "extract/extract.h"
#include "quantize/quantize.h"
#include "query/query.h"
#include "utils/score.h"
#include "utils/utils.h"


int main(int argc, char **argv) {
	ios::sync_with_stdio(0);

    // Initialize
    // extractAll();
    // quantizeAllData();

    extractAndQuantizeAll();

    if (argc < 2 || strcmp(argv[1], "test") == 0) 
    	runTest();
    else if (strcmp(argv[1], "server") == 0)
    	runServer();

    return 0;
}

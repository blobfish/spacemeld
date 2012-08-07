#include "smdservice.h"

int main(int argc, char *argv[])
{
    SMDService service(argc, argv);

    return service.exec();
}

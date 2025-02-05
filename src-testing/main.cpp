/**********************************************************************
 * This file is used for testing random stuff without running the
 * whole of SatDump, which comes in handy for debugging individual
 * elements before putting them all together in modules...
 *
 * If you are an user, ignore this file which will not be built by
 * default, and if you're a developper in need of doing stuff here...
 * Go ahead!
 *
 * Don't judge the code you might see in there! :)
 **********************************************************************/

#include "logger.h"
#include "common/ccsds/ccsds_1_0_proba/vcdu.h"
#include "common/ccsds/ccsds_1_0_proba/demuxer.h"
#include "common/ccsds/ccsds_1_0_proba/mpdu.h"
#include <fstream>

#include "common/image/image.h"
#include "common/repack.h"

int main(int /*argc*/, char *argv[])
{
    initLogger();

    std::ifstream idk_file(argv[1]);

    std::ofstream idk_file2(argv[2]);

    uint8_t buffer1[1279];

    // int sz = 0;

    ccsds::ccsds_1_0_proba::Demuxer demuxer_vcid1(1103, false);

    while (!idk_file.eof())
    {
        idk_file.read((char *)buffer1, 1279);

        auto vcdu = ccsds::ccsds_1_0_proba::parseVCDU(buffer1);

        //  logger->critical(vcdu.vcid);
        printf("VCID %d\n", vcdu.vcid);

        // 0, 1, 4, 5
        // 0 => Spec => 101
        // 1 => VNIR => 102
        // 4 => Lots
        // if (vcdu.vcid == 0)
        //     idk_file2.write((char *)buffer1, 1199);

        if (vcdu.vcid == 1)
        {
            memmove(&buffer1[10], &buffer1[17], 1016); // There is some sort of Insert zone

            // idk_file2.write((char *)&buffer1[0], 1199);

            // auto mpdu = ccsds::ccsds_1_0_proba::parseMPDU(buffer1);

            // logger->critical(mpdu.first_header_pointer);

            std::vector<ccsds::CCSDSPacket> pkts = demuxer_vcid1.work(buffer1);

            for (auto pkt : pkts)
            {
                // logger->critical("{:d} {:d} {:d}", pkt.header.apid, pkt.payload.size(), pkt.header.packet_length + 1);

                printf("APID %d\n", pkt.header.apid);

                if (pkt.header.apid == 793)
                {
                    // pkt.payload.resize(10000 - 6);
                    // idk_file2.write((char *)pkt.header.raw, 6);
                    // idk_file2.write((char *)pkt.payload.data(), 10000 - 6);

                    idk_file2.write((char *)pkt.header.raw, 6);
                    idk_file2.write((char *)pkt.payload.data(), pkt.payload.size());

                    printf("len %d\n", pkt.payload.size());

                    // if (pkt.payload.size() >= 65536)
                    // idk_file2.write((char *)pkt.payload.data(), 65536 - 4);
                }
            }
        }
    }
}

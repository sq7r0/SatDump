#pragma once

#include "core/module.h"
#include "instruments/chris/chris_reader.h"
#include "instruments/hrc/hrc_reader.h"
#include "instruments/swap/swap_reader.h"

#include "instruments/vegetation/vegs_reader.h"

namespace proba
{
    namespace instruments
    {
        class PROBAInstrumentsDecoderModule : public ProcessingModule
        {
        protected:
            enum proba_sat_t
            {
                PROBA_1,
                PROBA_2,
                PROBA_V,
            };

            proba_sat_t d_satellite;

            std::atomic<size_t> filesize;
            std::atomic<size_t> progress;

            // Readers
            std::unique_ptr<chris::CHRISReader> chris_reader;
            std::unique_ptr<hrc::HRCReader> hrc_reader;
            std::unique_ptr<swap::SWAPReader> swap_reader;
            std::unique_ptr<vegetation::VegetationS> vegs_reader1;
            std::unique_ptr<vegetation::VegetationS> vegs_reader2;
            std::unique_ptr<vegetation::VegetationS> vegs_reader3;
            std::unique_ptr<vegetation::VegetationS> vegs_reader4;
            std::unique_ptr<vegetation::VegetationS> vegs_reader5;
            std::unique_ptr<vegetation::VegetationS> vegs_reader6;

            // Statuses
            instrument_status_t chris_status = DECODING;
            instrument_status_t hrc_status = DECODING;
            instrument_status_t swap_status = DECODING;
            instrument_status_t vegs_status1 = DECODING;
            instrument_status_t vegs_status2 = DECODING;
            instrument_status_t vegs_status3 = DECODING;
            instrument_status_t vegs_status4 = DECODING;
            instrument_status_t vegs_status5 = DECODING;
            instrument_status_t vegs_status6 = DECODING;

        public:
            PROBAInstrumentsDecoderModule(std::string input_file, std::string output_file_hint, nlohmann::json parameters);
            void process();
            void drawUI(bool window);

        public:
            static std::string getID();
            virtual std::string getIDM() { return getID(); };
            static std::vector<std::string> getParameters();
            static std::shared_ptr<ProcessingModule> getInstance(std::string input_file, std::string output_file_hint, nlohmann::json parameters);
        };
    } // namespace amsu
} // namespace metop
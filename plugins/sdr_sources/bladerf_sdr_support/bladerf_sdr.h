#pragma once

#include "common/dsp_source_sink/dsp_sample_source.h"
#include <libbladeRF.h>
#include "logger.h"
#include "imgui/imgui.h"
#include "core/style.h"
#include <thread>

class BladeRFSource : public dsp::DSPSampleSource
{
protected:
    bool is_open = false, is_started = false;
    bladerf *bladerf_dev_obj;
    int bladerf_model = 0;
    int channel_cnt = 1;
    const bladerf_range *bladerf_range_samplerate;
    const bladerf_range *bladerf_range_bandwidth;
    const bladerf_range *bladerf_range_gain;

    int devs_cnt = 0;
    int selected_dev_id = 0;
    bladerf_devinfo *devs_list = NULL;

    int selected_samplerate = 0;
    std::string samplerate_option_str;
    std::vector<uint64_t> available_samplerates;
    uint64_t current_samplerate = 0;

    int channel_id = 0;
    int gain_mode = 1;
    int general_gain = 0;

    bool bias_enabled = false;

    void set_gains();
    void set_bias();

    static const int sample_buffer_size = 8192 * 4;
    int16_t sample_buffer[sample_buffer_size * 2];

    std::thread work_thread;
    bool thread_should_run = false, needs_to_run = false;
    void mainThread()
    {
        bladerf_metadata meta;

        while (thread_should_run)
        {
            if (needs_to_run)
            {
                if (bladerf_sync_rx(bladerf_dev_obj, sample_buffer, sample_buffer_size, &meta, 4000) != 0)
                    continue;
                volk_16i_s32f_convert_32f((float *)output_stream->writeBuf, sample_buffer, 4096.0f, sample_buffer_size * 2);
                output_stream->swap(sample_buffer_size);
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

public:
    BladeRFSource(dsp::SourceDescriptor source) : DSPSampleSource(source)
    {
        thread_should_run = true;
        work_thread = std::thread(&BladeRFSource::mainThread, this);
    }

    ~BladeRFSource()
    {
        stop();
        close();

        thread_should_run = false;
        logger->info("Waiting for the thread...");
        if (is_started)
            output_stream->stopWriter();
        if (work_thread.joinable())
            work_thread.join();
        logger->info("Thread stopped");
    }

    void set_settings(nlohmann::json settings);
    nlohmann::json get_settings();

    void open();
    void start();
    void stop();
    void close();

    void set_frequency(uint64_t frequency);

    void drawControlUI();

    void set_samplerate(uint64_t samplerate);
    uint64_t get_samplerate();

    static std::string getID() { return "bladerf"; }
    static std::shared_ptr<dsp::DSPSampleSource> getInstance(dsp::SourceDescriptor source) { return std::make_shared<BladeRFSource>(source); }
    static std::vector<dsp::SourceDescriptor> getAvailableSources();
};
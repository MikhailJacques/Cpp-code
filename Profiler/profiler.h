#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <mutex>
#include <thread>

struct ProfileResult
{
    std::string name;
    uint32_t thread_id;
    long long start;
    long long end;
};

struct ProfilerSession
{
    std::string name;
};

class Profiler
{
    private:

        ProfilerSession* m_current_session;
        std::ofstream m_output_stream;
        int m_profile_cnt;
        std::mutex m_lock;

    public:

        Profiler() : m_current_session(nullptr), m_profile_cnt(0) { }

        void BeginSession(const std::string& name, const std::string& filepath = "profile.json")
        {
            m_output_stream.open(filepath);
            WriteHeader();
            m_current_session = new ProfilerSession{ name };
        }

        void EndSession()
        {
            WriteFooter();
            m_output_stream.close();
            delete m_current_session;
            m_current_session = nullptr;
            m_profile_cnt = 0;
        }

        // This function writes an individually timed profile
        void WriteProfile(const ProfileResult& result)
        {
            std::lock_guard<std::mutex> lock(m_lock);

            if (m_profile_cnt++ > 0)
            {
                m_output_stream << ",";
            }

            std::string name = result.name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_output_stream << "{";
            m_output_stream << "\"cat\":\"function\",";
            m_output_stream << "\"dur\":" << (result.end - result.start) << ',';
            m_output_stream << "\"name\":\"" << name << "\",";
            m_output_stream << "\"ph\":\"X\",";
            m_output_stream << "\"pid\":0,";
            m_output_stream << "\"tid\":" << result.thread_id << ",";
            m_output_stream << "\"ts\":" << result.start;
            m_output_stream << "}";

            m_output_stream.flush();
        }

        void WriteHeader()
        {
            m_output_stream << "{\"otherData\": {},\"traceEvents\":[";
            m_output_stream.flush();
        }

        void WriteFooter()
        {
            m_output_stream << "]}";
            m_output_stream.flush();
        }

        static Profiler& Get()
        {
            static Profiler instance;

            return instance;
        }
};

// Scoped based timer
class Timer
{
    public:

        Timer(const char* name) : m_name(name), m_stopped_flag(false)
        {
            m_start_timestamp = std::chrono::high_resolution_clock::now();
        }

        ~Timer()
        {
            if (m_stopped_flag == false)
            {
                Stop();
            }
        }

        void Stop()
        {
            auto end_timestamp = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start_timestamp).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_timestamp).time_since_epoch().count();

            uint32_t thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());

            Profiler::Get().WriteProfile({ m_name, thread_id, start, end });

            m_stopped_flag = true;
        }

    private:

        const char* m_name;
        bool m_stopped_flag;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start_timestamp;     
};


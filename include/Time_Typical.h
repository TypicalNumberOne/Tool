#pragma once

//Timer.h 计时器
#ifndef _TIMERS_H_
#define _TIMERS_H_

#include "pch.h"
#include "Tchar_Typical.h"


namespace Typical_Tool {

	using day = std::chrono::duration<long long, std::ratio<60 * 60 * 24, 1>>; // 天
	using hour = std::chrono::duration<long long, std::ratio<60 * 60, 1>>;     // 时
	using min = std::chrono::duration<long long, std::ratio<60, 1>>;           // 分
	using sec = std::chrono::seconds;                                          // 秒
	using ms = std::chrono::milliseconds;                                      // 毫秒
	using us = std::chrono::microseconds;                                      // 微秒
	using ns = std::chrono::nanoseconds;                                       // 纳秒

	template <typename Target>
	std::string TimeMeasureToString() {
		if constexpr (std::is_same<Target, day>::value) {
			return "天";
		}
		else if constexpr (std::is_same<Target, hour>::value) {
			return "时";
		}
		else if constexpr (std::is_same<Target, min>::value) {
			return "分";
		}
		else if constexpr (std::is_same<Target, sec>::value) {
			return "秒";
		}
		else if constexpr (std::is_same<Target, ms>::value) {
			return "毫秒";
		}
		else if constexpr (std::is_same<Target, us>::value) {
			return "微秒";
		}
		else if constexpr (std::is_same<Target, ns>::value) {
			return "纳秒";
		}
		else {
			return "未知单位";
		}
	}

	//计时器
	class Timer
	{
	private:
		std::chrono::steady_clock::time_point InitTime; //初始的时间
		std::vector<std::chrono::steady_clock::time_point> TimerContainer; //计时器 集合(Container)

		static bool showLog;

	public:
		Timer(bool _showLog = false)
		{
			InitTime = GetTime();
			TimerContainer.push_back(InitTime);

			showLog = _showLog;
		}

	public:
		static void SetShowLog(bool _showLog = false);

		static std::chrono::steady_clock::time_point GetTime();

		void AddTimer();
		void AddTimer(const std::chrono::steady_clock::time_point& time);
		void AddTimer(std::chrono::steady_clock::time_point&& time);

		void SetTimer(const std::chrono::steady_clock::time_point& time, int Location);
		void SetTimer(std::chrono::steady_clock::time_point&& time, int Location);

		std::chrono::steady_clock::time_point GetTimer(int Location);

		std::vector<std::chrono::steady_clock::time_point> GetTimerContainer();
		int GetTimerSize();

	public:
		long long ComputTime_FirstToEnd();
		long long ComputTime_FrontToBack();
		long long ComputTime(int LocationBegin, int LocationEnd);

		// time: 时间
		// transformTarget: 转换前的时间计量
		// transformResult: 转换后的时间计量
		// return 转换后的时间
		template<class Target = sec, class Result = ms>
		static long long TransformTimes(const long long& time)
		{
			return std::chrono::duration_cast<Result>(Target(time)).count();
		}
	};

	//时间
	class Time {
	public:
		static bool showLog;

	public:
		static void SetShowLog(bool _showLog);

		template<class Target>
		static void sleep_s(long long _Number)
		{
			std::this_thread::sleep_for(Target(_Number));

			if (showLog) {
				Terr << ANSIESC_YELLOW << "休眠: [" << _Number << "]" << TimeMeasureToString<Target>() << ANSIESC_RESET << endl;
			}
		}
		static void sleep(long long _sec)
		{
			std::this_thread::sleep_for(std::chrono::seconds(_sec));
			if (showLog) {
				Terr << ANSIESC_YELLOW << "休眠: [" << _sec << "]秒" << ANSIESC_RESET << endl;
			}
		}
		template<class Target>
		static void wait_s(long long _Number)
		{
			Timer timer;
			long long timeTarget = (std::chrono::duration_cast<Target>(timer.GetTime().time_since_epoch()) + Target(_Number)).count();
			while (timeTarget > std::chrono::duration_cast<Target>(timer.GetTime().time_since_epoch()).count()) {
			}
			if (showLog) {
				Terr << ANSIESC_YELLOW << "等待: [" << _Number << "]" << TimeMeasureToString<Target>() << ANSIESC_RESET << endl;
			}
		}
		static void wait(long long _sec)
		{
			Timer timer;
			long long timeTarget = (std::chrono::duration_cast<std::chrono::seconds>(timer.GetTime().time_since_epoch()) + std::chrono::seconds(_sec)).count();
			while (timeTarget > std::chrono::duration_cast<std::chrono::seconds>(timer.GetTime().time_since_epoch()).count()) {
			}if (showLog) {
				Terr << ANSIESC_YELLOW << "等待: [" << _sec << "]秒" << ANSIESC_RESET << endl;
			}
		}


		static void FormatTime(Tstr& text, const Tstr& timeFormat = "%Y-%m-%d %H:%M:%S",
			const Tstr& textLeftFormat = "[", const Tstr& textRigthFormat = "]")
		{
			std::chrono::system_clock::time_point now = std::chrono::system_clock::now();;
			// 获取当前时间点（自epoch以来的时间）
			// 将时间点转换为time_t（用于localtime函数）
			std::time_t ttm = std::chrono::system_clock::to_time_t(now);
			// 使用localtime函数将time_t转换为本地时间（std::tm结构）
			std::tm* now_tm = std::localtime(&ttm);

			// 使用 std::put_time 格式化时间
			Tostringstream oss;
			oss << std::put_time(now_tm, timeFormat.c_str()); // 自定义时间格式

			//不需要修饰字符时, 直接返回格式化后的时间文本
			if (textLeftFormat == "" && textRigthFormat == "") {
				text = oss.str() + text;
			}
			else {
				text = textLeftFormat + oss.str() + textRigthFormat + text;
			}
		}

		static Tstr GetFormatTime(const Tstr& timeFormat = "%Y-%m-%d %H:%M:%S",
			const Tstr& textLeftFormat = "[", const Tstr& textRigthFormat = "]")
		{
			std::chrono::system_clock::time_point now = std::chrono::system_clock::now();;
			// 获取当前时间点（自epoch以来的时间）
			// 将时间点转换为time_t（用于localtime函数）
			std::time_t ttm = std::chrono::system_clock::to_time_t(now);
			// 使用localtime函数将time_t转换为本地时间（std::tm结构）
			std::tm* now_tm = std::localtime(&ttm);

			// 使用 std::put_time 格式化时间
			Tostringstream oss;
			oss << std::put_time(now_tm, timeFormat.c_str()); // 自定义时间格式

			//不需要修饰字符时, 直接返回格式化后的时间文本
			if (textLeftFormat == "" && textRigthFormat == "") {
				return oss.str();
			}
			else {
				return textLeftFormat + oss.str() + textRigthFormat;
			}
		}
	};
}

#endif
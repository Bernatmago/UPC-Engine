#pragma once

#include "imgui.h"

class Console
{
public:
	void Clear() {
		buff.clear();
		line_offsets.clear();
		line_offsets.push_back(0);
	}

	// Apply printf-style warnings to our formatting functions.
	void AddLog(const char file[], int line, const char* format, ...) IM_FMTARGS(2)
	{
		const static int max_log_size = 4096;

		static char tmp_string[max_log_size];
		tmp_string[0] = '\n';
		static char tmp_string2[max_log_size];

		int old_size = buff.size();

		va_list args;
		va_start(args, format);
		vsprintf_s(&tmp_string[1], max_log_size-1, format, args);
		va_end(args);

		buff.append(tmp_string);
		for (int new_size = buff.size(); old_size < new_size; old_size++)
			if (buff[old_size] == '\n')
				line_offsets.push_back(old_size + 1);

		// Visual Studio prints log source
		sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, &tmp_string[1]);		
		OutputDebugString(tmp_string2);
	}

	Console() {
		autoscroll = true;
		Clear();
	}

	~Console() {
		Clear();
	}

	void Draw(bool* p_open) {
		static bool first_run = true;

		if (first_run) {
			ImVec2 window_size = ImGui::GetMainViewport()->Size;
			ImVec2 initial_size = ImVec2(window_size.x * 0.35f, window_size.y * 0.20f);
			ImVec2 initial_pos = ImVec2(0.0f, window_size.y - initial_size.y);
			ImGui::SetNextWindowSize(initial_size, ImGuiCond_Once);
			ImGui::SetNextWindowPos(initial_pos, ImGuiCond_Once);
			first_run = false;
		}

		if (!ImGui::Begin("Console", p_open))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto-scroll", &autoscroll);
			ImGui::EndPopup();
		}

		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");

		ImGui::SameLine();
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		filter.Draw("Filter", -100.0f);

		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (clear)
			Clear();
		if (copy)
			ImGui::LogToClipboard();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf_start = buff.begin();
		const char* buf_end = buff.end();

		if (filter.IsActive())
		{
			for (int line_no = 0; line_no < line_offsets.Size; line_no++)
			{
				const char* line_start = buf_start + line_offsets[line_no];
				const char* line_end = (line_no + 1 < line_offsets.Size) ? (buf_start + line_offsets[line_no + 1] - 1) : buf_end;
				if (filter.PassFilter(line_start, line_end))
					ImGui::TextUnformatted(line_start, line_end);
			}
		}
		else
		{
			ImGuiListClipper clipper;
			clipper.Begin(line_offsets.Size);
			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					const char* line_start = buf_start + line_offsets[line_no];
					const char* line_end = (line_no + 1 < line_offsets.Size) ? (buf_start + line_offsets[line_no + 1] - 1) : buf_end;
					ImGui::TextUnformatted(line_start, line_end);
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		if (autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
		ImGui::End();
	}
private:
	ImGuiTextBuffer buff;
	ImGuiTextFilter filter;
	ImVector<int> line_offsets;
	bool autoscroll;
};

extern Console* Logger;
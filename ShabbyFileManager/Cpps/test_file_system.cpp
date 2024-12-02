#include <functional>
#include <iostream>
#include "FileManager.h"
#include "json.hpp"

using json = nlohmann::json;

class SingleAutomata {
public:
    std::string id;
    std::string init_status;
    std::string transfer_function;
    std::string terminate_set;
};

class GraphModel {
public:
    std::string model_name;
    std::vector<SingleAutomata> automatas;
};

class ForCallBack
{
public:
    bool FillUp(const std::string& json_string, GraphModel& graph_model) {
        json json_obj;
        if (!file_manager.TransStr2JsonObject(json_string, json_obj)) {
            std::cerr << "Failed to parse JSON: " << json_string << std::endl;
            return false;
        }

        switch (current_state) {
        case State::WaitingForModel:
            if (json_obj.contains("model")) {
                current_state = State::ReadingModelName;
                return FillUp(json_string, graph_model);
            }
            break;
        case State::ReadingModelName:
            if (json_obj["model"].is_array())
                for (int i = 0; i < json_obj["model"].size(); ++i)
                    if (json_obj["model"][i].contains("name"))
                    {
                        graph_model.model_name = json_obj["model"][i]["name"];
                        break;
                    }
            current_state = State::WaitingForAutomata;
            break;
        case State::WaitingForAutomata:
            if (json_obj.contains("automata")) {
                current_state = State::ReadingAutomataId;
                graph_model.automatas.emplace_back();
                return FillUp(json_string, graph_model);
            }
            break;
        case State::ReadingAutomataId:
            if (json_obj["automata"].is_array())
                for (int i = 0; i < json_obj["automata"].size(); ++i)
                    if (json_obj["automata"][i].contains("id"))
                    {
                        graph_model.automatas.back().id = json_obj["automata"][i]["id"];
                        break;
                    }
            current_state = State::ReadingInitStatus;
            break;
        case State::ReadingInitStatus:
            if (json_obj["init_status"].is_array())
                for (int i = 0; i < json_obj["init_status"].size(); ++i)
                    if (json_obj["init_status"][i].contains("param"))
                    {
                        graph_model.automatas.back().init_status = json_obj["init_status"][i]["param"];
                        break;
                    }
            current_state = State::ReadingTransferFunction;
            break;
        case State::ReadingTransferFunction:
            if (json_obj["transfer_function"].is_array())
                for (int i = 0; i < json_obj["transfer_function"].size(); ++i)
                    if (json_obj["transfer_function"][i].contains("func"))
                    {
                        graph_model.automatas.back().transfer_function = json_obj["transfer_function"][i]["func"];
                        break;
                    }
            current_state = State::ReadingTerminateSet;
            break;
        case State::ReadingTerminateSet:
            if (json_obj["terminate_set"].is_array())
                for (int i = 0; i < json_obj["terminate_set"].size(); ++i)
                    if (json_obj["terminate_set"][i].contains("set"))
                    {
                        graph_model.automatas.back().terminate_set = json_obj["terminate_set"][i]["set"];
                        break;
                    }
            current_state = State::WaitingForAutomata;
            break;
        default:
            throw std::logic_error("Invalid state");
        }
        return true;
    }

private:
    enum class State {
        WaitingForModel,
        ReadingModelName,
        WaitingForAutomata,
        ReadingAutomataId,
        ReadingInitStatus,
        ReadingTransferFunction,
        ReadingTerminateSet
    };

    State current_state = State::WaitingForModel;
    FileManager file_manager;
};

int main()
{
    FileManager fm;
    GraphModel gm;
    ForCallBack callback;

    auto bound_func = std::bind(&ForCallBack::FillUp, &callback, std::placeholders::_1, std::placeholders::_2);
    bool traverse_result = fm.TransXml2Class<GraphModel>("./Resources/segment_automata.xml", gm, bound_func);

    if (!traverse_result) {
        std::cerr << "Failed to traverse XML to class." << std::endl;
    }
    int i = 0;
    return 0;
}


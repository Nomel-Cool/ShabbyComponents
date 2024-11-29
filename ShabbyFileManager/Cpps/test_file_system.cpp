#include "FileManager.h"
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

    bool FillUp(const std::string& json_string) {
        json json_obj;
        if (!json_parser.DeSerialize(json_string, json_obj)) {
            std::cerr << "Failed to parse JSON: " << json_string << std::endl;
            return false;
        }

        switch (current_state) {
        case State::WaitingForModel:
            if (json_obj.contains("model")) {
                current_state = State::ReadingModelName;
                return FillUp(json_string); // Re-process this JSON string in the new state
            }
            break;
        case State::ReadingModelName:
            if (json_obj["model"].is_array())
                for (int i = 0; i < json_obj["model"].size(); ++i)
                    if (json_obj["model"][i].contains("name"))
                    {
                        model_name = json_obj["model"][i]["name"];
                        break;
                    }
            current_state = State::WaitingForAutomata;
            break;
        case State::WaitingForAutomata:
            if (json_obj.contains("automata")) {
                current_state = State::ReadingAutomataId;
                automatas.emplace_back();
                return FillUp(json_string); // Re-process this JSON string in the new state
            }
            break;
        case State::ReadingAutomataId:
            if (json_obj["automata"].is_array())
                for (int i = 0; i < json_obj["automata"].size(); ++i)
                    if (json_obj["automata"][i].contains("id"))
                    {
                        automatas.back().id = json_obj["automata"][i]["id"];
                        break;
                    }
            current_state = State::ReadingInitStatus;
            break;
        case State::ReadingInitStatus:
            if (json_obj["init_status"].is_array())
                for (int i = 0; i < json_obj["init_status"].size(); ++i)
                    if (json_obj["init_status"][i].contains("param"))
                    {
                        automatas.back().init_status = json_obj["init_status"][i]["param"];
                        break;
                    }
            current_state = State::ReadingTransferFunction;
            break;
        case State::ReadingTransferFunction:
            if (json_obj["transfer_function"].is_array())
                for (int i = 0; i < json_obj["transfer_function"].size(); ++i)
                    if (json_obj["transfer_function"][i].contains("func"))
                    {
                        automatas.back().transfer_function = json_obj["transfer_function"][i]["func"];
                        break;
                    }
            current_state = State::ReadingTerminateSet;
            break;
        case State::ReadingTerminateSet:
            if (json_obj["terminate_set"].is_array())
                for (int i = 0; i < json_obj["terminate_set"].size(); ++i)
                    if (json_obj["terminate_set"][i].contains("set"))
                    {
                        automatas.back().terminate_set = json_obj["terminate_set"][i]["set"];
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
    JsonParser json_parser;
};

int main()
{
    FileManager fm;
    GraphModel gm;
    auto traverse_result = fm.TransXml2Class("segment_automata.xml", gm);
    int i = 0;
    return 0;
}

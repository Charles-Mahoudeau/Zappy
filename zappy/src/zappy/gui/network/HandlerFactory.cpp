/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** HandlerFactory
*/

#include "zappy/gui/network/HandlerFactory.hpp"

#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/handlers/BctHandler.hpp"
#include "zappy/gui/network/handlers/EboHandler.hpp"
#include "zappy/gui/network/handlers/EdiHandler.hpp"
#include "zappy/gui/network/handlers/EnwHandler.hpp"
#include "zappy/gui/network/handlers/MszHandler.hpp"
#include "zappy/gui/network/handlers/PbcHandler.hpp"
#include "zappy/gui/network/handlers/PdiHandler.hpp"
#include "zappy/gui/network/handlers/PdrHandler.hpp"
#include "zappy/gui/network/handlers/PexHandler.hpp"
#include "zappy/gui/network/handlers/PfkHandler.hpp"
#include "zappy/gui/network/handlers/PgtHandler.hpp"
#include "zappy/gui/network/handlers/PicHandler.hpp"
#include "zappy/gui/network/handlers/PieHandler.hpp"
#include "zappy/gui/network/handlers/PinHandler.hpp"
#include "zappy/gui/network/handlers/PlvHandler.hpp"
#include "zappy/gui/network/handlers/PnwHandler.hpp"
#include "zappy/gui/network/handlers/PpoHandler.hpp"
#include "zappy/gui/network/handlers/SbpHandler.hpp"
#include "zappy/gui/network/handlers/SegHandler.hpp"
#include "zappy/gui/network/handlers/SgtHandler.hpp"
#include "zappy/gui/network/handlers/SmgHandler.hpp"
#include "zappy/gui/network/handlers/SstHandler.hpp"
#include "zappy/gui/network/handlers/SucHandler.hpp"
#include "zappy/gui/network/handlers/TnaHandler.hpp"

namespace zappy::gui::network {

HandlerMap HandlerFactory::create(game::GameState& state) {
    return {
        {"msz", handlers::MszHandler{state}}, {"bct", handlers::BctHandler{state}},
        {"tna", handlers::TnaHandler{state}}, {"pnw", handlers::PnwHandler{state}},
        {"ppo", handlers::PpoHandler{state}}, {"plv", handlers::PlvHandler{state}},
        {"pin", handlers::PinHandler{state}}, {"pex", handlers::PexHandler{state}},
        {"pbc", handlers::PbcHandler{state}}, {"pic", handlers::PicHandler{state}},
        {"pie", handlers::PieHandler{state}}, {"pfk", handlers::PfkHandler{state}},
        {"pdr", handlers::PdrHandler{state}}, {"pgt", handlers::PgtHandler{state}},
        {"pdi", handlers::PdiHandler{state}}, {"enw", handlers::EnwHandler{state}},
        {"ebo", handlers::EboHandler{state}}, {"edi", handlers::EdiHandler{state}},
        {"sgt", handlers::SgtHandler{state}}, {"sst", handlers::SstHandler{state}},
        {"seg", handlers::SegHandler{state}}, {"smg", handlers::SmgHandler{state}},
        {"suc", handlers::SucHandler{state}}, {"sbp", handlers::SbpHandler{state}},
    };
}

}  // namespace zappy::gui::network

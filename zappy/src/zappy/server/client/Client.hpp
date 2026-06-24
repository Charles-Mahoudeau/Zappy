/*
** EPITECH PROJECT, 2026
** IClient
** File description:
** IClient header
*/

#pragma once

#include <cstdint>
#include <optional>
#include <queue>
#include <string>
#include <string_view>

#include "zappy/server/Timer.hpp"
#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::server {

class Client {
  public:
    /**
     * @brief Represents all possible client types in the system.
     *
     * @details Each client connected to the server is assigned a type that
     * determines its role and behavior within the application.
     *
     * @note A client starts as @c kUnknown until it identifies itself.
     * A @c kUnknown client **must** be assigned a team before it can be
     * promoted to another type.
     *
     * ### Types overview:
     * | Value      | Description                              |
     * |------------|------------------------------------------|
     * | kPlayer    | An AI-controlled client (game logic)     |
     * | kGui       | A graphical client (rendering/display)   |
     * | kUnknown   | Default type, pending identification     |
     */
    enum class Type : std::uint8_t {
        kPlayer,   ///< AI-controlled client managing game logic.
        kGui,      ///< Graphical client responsible for rendering.
        kUnknown,  ///< Default unidentified client; requires a team assignment before promotion.
    };

    Client(net::SocketRegistry& socketRegister, network::Address address, Timer& timer);
    ~Client();

    Client(const Client&) = delete;
    Client(Client&&) = delete;
    Client& operator=(const Client&) = delete;
    Client& operator=(Client&&) = delete;

    /**
     * @brief Polls the underlying socket and enqueues any newly received messages.
     *
     * @details Looks up the socket associated with this client's address in the
     * registry and drains all pending messages into the internal request queue,
     * respecting the @c kMaxRequests cap.
     *
     * @return @c true  if the socket was found and the poll succeeded.
     * @return @c false if no socket is registered for this client's address,
     *         indicating the connection has been lost.
     */
    bool update();

    /**
     * @brief Returns the remote network address of this client.
     *
     * @return A const reference to the @c network::Address identifying this client.
     */
    [[nodiscard]] const network::Address& address() const;

    /**
     * @brief Returns the current type of this client.
     *
     * @return The @c Type enum value assigned to this client.
     *
     * @see changeType()
     */
    [[nodiscard]] Type type() const;

    /**
     * @brief Reassigns the type of this client.
     *
     * @details Used during the handshake phase to promote a @c kUnknown client
     * to its definitive role (@c kPlayer or @c kGui).
     *
     * @param type The new @c Type to assign to this client.
     */
    void changeType(Type type);

    /**
     * @brief Enqueues an incoming request string for later processing.
     *
     * @details The request is silently dropped if the queue already holds
     * @c kMaxRequests entries, acting as a simple back-pressure mechanism.
     *
     * @param msg The raw request string received from the client.
     */
    void addRequest(std::string msg);

    /**
     * @brief Dequeues and returns the next pending request, if any.
     *
     * @details If a timeout is currently active, it is decremented and
     * @c std::nullopt is returned without consuming any request. Once the
     * timeout reaches zero, requests are served in FIFO order.
     *
     * @return The oldest pending request string, or @c std::nullopt if the
     *         queue is empty or the client is still in its timeout period.
     */
    std::optional<std::string> nextRequest();

    /**
     * @brief Sends a message to this client over its registered socket.
     *
     * @details Looks up the socket in the registry and forwards the message.
     * Any @c SocketError thrown during the send is caught and converted into
     * a @c false return value so callers can handle disconnections gracefully.
     *
     * @param msg A non-owning view of the message string to transmit.
     *
     * @return @c true  if the message was sent successfully.
     * @return @c false if the socket was not found or a socket error occurred.
     */
    [[nodiscard]] bool sendMessage(std::string_view msg) const;

    /**
     * @brief check if the client currently have a action running
     * @return bool true if in timeout. false otherwise
     */
    [[nodiscard]] bool inTimeout() const { return this->_timeoutId != 0; }

    /**
     * @brief set a timeout to N tick
     * @param time nb tick to timeout
     * @return bool true if successfully timed out. false if already in timeout
     */
    bool setTimeout(int time);

    /**
     * @brief remove the client timeout from the Timer
     */
    void removeTimeout();

  private:
    Timer& _timer;
    std::uint64_t _timeoutId = 0;
    std::queue<std::string> _requests;
    Type _type = Client::Type::kUnknown;
    network::Address _addr;
    net::SocketRegistry& _socketsRegistery;

    /**
     * @brief Max Number requests related to a client.
     *        if more is sended before the currents requests been
     *        consumed. The sended request will be ignored
     */
    static constexpr int kMaxRequests = 10;
};

}  // namespace zappy::server

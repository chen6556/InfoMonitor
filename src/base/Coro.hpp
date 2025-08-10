#pragma once
#include <coroutine>
#include <QNetworkReply>


struct NetworkReplyAwaitable
{
    QMetaObject::Connection conn;
    QNetworkReply *reply = nullptr;
    //! 协程的句柄
    std::coroutine_handle<> awaiting;

    NetworkReplyAwaitable(QNetworkReply *r)
        : reply(r) {};

    ~NetworkReplyAwaitable() = default;

    bool await_ready()
    {
        return reply->isFinished();
    }

    void await_suspend(std::coroutine_handle<> h)
    {
        awaiting = h;
        reply->connect(reply, &QNetworkReply::finished, [&]() { awaiting.resume(); });
    }

    // 返回结果
    QNetworkReply *await_resume()
    {
        return reply;
    }
};

struct NetworkReplyCoro
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    handle_type handle;

    //! promise_type就是承诺对象，承诺对象用于协程内外交流
    struct promise_type
    {
        promise_type() = default;

        ~promise_type() = default;

        auto get_return_object()
        {
            return NetworkReplyCoro{handle_type::from_promise(*this)};
        }

        auto initial_suspend()
        {
            return std::suspend_never{};
        }

        void return_void() {}

        auto final_suspend() noexcept
        {
            return std::suspend_never{};
        }

        void unhandled_exception()
        {
            std::exit(1);
        }

        NetworkReplyAwaitable await_transform(QNetworkReply *r)
        {
            return NetworkReplyAwaitable(r);
        }
    };

    NetworkReplyCoro(handle_type h)
        : handle(h) {}

    NetworkReplyCoro(const NetworkReplyCoro &) = delete;

    NetworkReplyCoro(NetworkReplyCoro &&s)
        : handle(s.handle)
    {
        s.handle = nullptr;
    }

    ~NetworkReplyCoro() = default;

    NetworkReplyCoro &operator=(const NetworkReplyCoro &) = delete;

    NetworkReplyCoro &operator=(NetworkReplyCoro &&s)
    {
        handle = s.handle;
        s.handle = nullptr;
        return *this;
    }

    //! 恢复协程，返回是否结束
    bool move_next()
    {
        handle.resume();
        return handle.done();
    }
};



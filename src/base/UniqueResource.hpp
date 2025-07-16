#pragma once
#include <QTimer>
#include <QNetworkAccessManager>


struct UniqueResource
{
    QTimer Timer1, Timer60, Timer180, Timer300, Timer600, Timer1800, Timer3600; // second(s)
    QNetworkAccessManager NetworkManager;

    static UniqueResource &Resource();

private:
    UniqueResource();

    ~UniqueResource() = default;

    UniqueResource(const UniqueResource &) = delete;

    const UniqueResource &operator=(const UniqueResource &) = delete;
};
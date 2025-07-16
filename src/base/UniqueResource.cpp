#include "UniqueResource.hpp"


UniqueResource &UniqueResource::Resource()
{
    static UniqueResource resource;
    return resource;
}

UniqueResource::UniqueResource()
{
    NetworkManager.setAutoDeleteReplies(true);

    Timer1.start(1'000);

    Timer600.start(600'000);

    Timer3600.start(3'600'000);
}
#include "bulletshapemanager.hpp"

#include <components/vfs/manager.hpp>

#include <components/nifbullet/bulletnifloader.hpp>

#include "bulletshape.hpp"

namespace Resource
{

BulletShapeManager::BulletShapeManager(const VFS::Manager* vfs)
    : mVFS(vfs)
{

}

BulletShapeManager::~BulletShapeManager()
{

}

osg::ref_ptr<BulletShapeInstance> BulletShapeManager::createInstance(const std::string &name)
{
    std::string normalized = name;
    mVFS->normalizeFilename(normalized);

    osg::ref_ptr<BulletShape> shape;
    Index::iterator it = mIndex.find(normalized);
    if (it == mIndex.end())
    {
        Files::IStreamPtr file = mVFS->get(normalized);

        // TODO: add support for non-NIF formats
        size_t extPos = normalized.find_last_of('.');
        std::string ext;
        if (extPos != std::string::npos && extPos+1 < normalized.size())
            ext = normalized.substr(extPos+1);

        if (ext != "nif")
            return NULL;

        NifBullet::BulletNifLoader loader;
        // might be worth sharing NIFFiles with SceneManager in some way
        shape = loader.load(Nif::NIFFilePtr(new Nif::NIFFile(file, normalized)));

        mIndex[normalized] = shape;
    }
    else
        shape = it->second;

    osg::ref_ptr<BulletShapeInstance> instance = shape->makeInstance();
    return instance;
}

}
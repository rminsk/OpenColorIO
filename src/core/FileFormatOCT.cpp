/*
Copyright (c) 2003-2010 Sony Pictures Imageworks Inc., et al.
All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
* Neither the name of Sony Pictures Imageworks nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Copyright (c) 2014 Method Studios.  All Rights Reserved.  */

#include <OpenColorIO/OpenColorIO.h>
#include "FileTransform.h"
#include "OCIOYaml.h"
#include "OpBuilders.h"

OCIO_NAMESPACE_ENTER
{
    ////////////////////////////////////////////////////////////////
    
    namespace
    {
        class LocalCachedFile : public CachedFile
        {
        public:
            LocalCachedFile() {}
            ~LocalCachedFile() {}
            
            TransformRcPtr transform_;
        };
        
        typedef OCIO_SHARED_PTR<LocalCachedFile> LocalCachedFileRcPtr;

        class LocalFileFormat : public FileFormat
        {
        public:
            ~LocalFileFormat() {}

            virtual void GetFormatInfo(FormatInfoVec & formatInfoVec) const;
            virtual CachedFileRcPtr Read(std::istream & istream) const;
            virtual void BuildFileOps(OpRcPtrVec & ops,
                                      const Config& config,
                                      const ConstContextRcPtr & context,
                                      CachedFileRcPtr untypedCachedFile,
                                      const FileTransform& fileTransform,
                                      TransformDirection dir) const;
        };

        void LocalFileFormat::GetFormatInfo(FormatInfoVec & formatInfoVec) const
        {
            FormatInfo info;
            info.name = "ocio_transform";
            info.extension = "oct";
            info.capabilities = FORMAT_CAPABILITY_READ;
            formatInfoVec.push_back(info);
        }

        CachedFileRcPtr LocalFileFormat::Read(std::istream & istream) const
        {
            TransformRcPtr transform;
            OCIOYaml::open(istream, transform);

            LocalCachedFileRcPtr cachedFile =
                LocalCachedFileRcPtr(new LocalCachedFile());
            cachedFile->transform_ = transform;
            return cachedFile;
        }

        void LocalFileFormat::BuildFileOps(OpRcPtrVec & ops,
                                           const Config & config,
                                           const ConstContextRcPtr & context,
                                           CachedFileRcPtr untypedCachedFile,
                                           const FileTransform & fileTransform,
                                           TransformDirection dir) const
        {
            const LocalCachedFileRcPtr cachedFile = DynamicPtrCast<LocalCachedFile>(untypedCachedFile);

            if(!cachedFile) // This should never happen.
            {
                throw Exception("Cannot build OCT Ops. Invalid cache type.");
            }

            TransformDirection newDir = CombineTransformDirections(dir,
                fileTransform.getDirection());

            BuildOps(ops, config, context, cachedFile->transform_, newDir);
        }
    }

    FileFormat * CreateFileFormatOCT()
    {
        return new LocalFileFormat();
    }
}
OCIO_NAMESPACE_EXIT

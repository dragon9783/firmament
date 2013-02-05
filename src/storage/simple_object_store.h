// The Firmament project
// Copyright (c) 2011-2012 Malte Schwarzkopf <malte.schwarzkopf@cl.cam.ac.uk>
//
// Stub header for object store. This currently supports a very simple in-memory
// key-value store.

#ifndef FIRMAMENT_ENGINE_SIMPLE_OBJECT_STORE_H
#define FIRMAMENT_ENGINE_SIMPLE_OBJECT_STORE_H

#include <string>
#include <map>

#include "base/common.h"
#include "base/types.h"

#include "storage/types.h"
#include "storage/object_store_interface.h"

#include "base/reference_desc.pb.h"

#include "platforms/unix/common.h"


#include "misc/messaging_interface.h"

#include "platforms/common.h"

#include "platforms/unix/stream_sockets_adapter.h"
#include "platforms/unix/stream_sockets_adapter-inl.h"

#include "platforms/unix/stream_sockets_channel.h"
#include "platforms/unix/stream_sockets_channel-inl.h"

#include "messages/base_message.pb.h"
#include "messages/storage_message.pb.h"
#include "base/common.h"
#include "base/types.h"
#include "base/job_desc.pb.h"
#include "base/task_desc.pb.h"
#include "base/reference_desc.pb.h"
#include "base/resource_desc.pb.h"
#include "base/resource_topology_node_desc.pb.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include "misc/map-util.h"



namespace firmament {
  namespace store {

    using platform_unix::streamsockets::StreamSocketsChannel;
    using platform_unix::streamsockets::StreamSocketsAdapter;
    using namespace std;
    using namespace boost; 

    class Cache;

    class StorageInfo;

    class SimpleObjectStore : public ObjectStoreInterface {
    public:
      SimpleObjectStore(ResourceID_t uuid);
      ~SimpleObjectStore();

      virtual ostream& ToString(ostream* stream) const {
        return *stream << "<SimpleObjectStore, containing ";
      }
      void HandleIncomingMessage(BaseMessage *bm);
      void HandleIncomingReceiveError(const boost::system::error_code& error,
              const string& remote_endpoint);
      void HandleStorageRegistrationRequest(const StorageRegistrationMessage& msg);
      void HandleIncomingHeartBeat(const StorageHeartBeatMessage& msg );
 
      void setUpCommunication();
      void printTopology();

      void obtain_object_remotely(DataObjectID_t id);

   
      void HeartBeatMasterTask(); 
      void sendHeartBeatMasterTask(); 
      
      
      StorageInfo* infer_best_location(ReferenceDescriptor* rd); 

    protected:
      shared_ptr<StreamSocketsAdapter<BaseMessage> > message_adapter_;
      NodeTable_t peers; /* Channel interfaces, etc. */
      NodeTable_t nodes; /* Concatenate global information
                          about all the nodes.
                          */

    private:
      shared_ptr<Cache> cache;
      
      /* Currently only exchange heartbeats with peers*/
      long int peer_heartbeat_frequency; 
     
      void createSharedBuffer(size_t size);
      void HeartBeatTask();
      void sendHeartbeat(); 
   
    }; 
    
  } // namespace store
} // namespace firmament

#endif  // FIRMAMENT_ENGINE_SIMPLE_OBJECT_STORE_H

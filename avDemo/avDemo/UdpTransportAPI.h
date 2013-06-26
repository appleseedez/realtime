//
//  UdpTransportAPI.h
//  avDemo
//
//  Created by chenjianjun on 13-6-19.
//  Copyright (c) 2013年 free. All rights reserved.
//

#ifndef __avDemo__UdpTransportAPI__
#define __avDemo__UdpTransportAPI__

#include <iostream>
#include "webrtc/test/channel_transport/udp_transport.h"
#include "webrtc/video_engine/include/vie_network.h"
#include "webrtc/video_engine/vie_defines.h"
#include "webrtc/voice_engine/include/voe_network.h"


class VoiceTransport : public webrtc::test::UdpTransportData {
public:
    VoiceTransport(webrtc::VoENetwork* voe_network, int channel);
    
    virtual ~VoiceTransport();
    
    // Start implementation of UdpTransportData.
    void IncomingRTPPacket(const int8_t* incoming_rtp_packet,
                           const int32_t packet_length,
                           const char* /*from_ip*/,
                           const uint16_t /*from_port*/);
    
    void IncomingRTCPPacket(const int8_t* incoming_rtcp_packet,
                            const int32_t packet_length,
                            const char* /*from_ip*/,
                            const uint16_t /*from_port*/);
    // End implementation of UdpTransportData.
    
    // Specifies the ports to receive RTP packets on.
    int SetLocalReceiver(uint16_t rtp_port);
    
    // Specifies the destination port and IP address for a specified channel.
    int SetSendDestination(const char* ip_address, uint16_t rtp_port);
    
private:
    int channel_;
    webrtc::VoENetwork* voe_network_;
    webrtc::test::UdpTransport* socket_transport_;
};

// Helper class for VideoEngine tests.
class VideoTransport : public webrtc::test::UdpTransportData {
public:
    VideoTransport(webrtc::ViENetwork* vie_network, int channel);
    
    virtual  ~VideoTransport();
    
    // Start implementation of UdpTransportData.
    void IncomingRTPPacket(const int8_t* incoming_rtp_packet,
                           const int32_t packet_length,
                           const char* /*from_ip*/,
                           const uint16_t /*from_port*/);
    
    void IncomingRTCPPacket(const int8_t* incoming_rtcp_packet,
                            const int32_t packet_length,
                            const char* /*from_ip*/,
                            const uint16_t /*from_port*/);
    // End implementation of UdpTransportData.
    
    // Specifies the ports to receive RTP packets on.
    int SetLocalReceiver(uint16_t rtp_port);
    
    // Specifies the destination port and IP address for a specified channel.
    int SetSendDestination(const char* ip_address, uint16_t rtp_port);
    
private:
    int channel_;
    webrtc::ViENetwork* vie_network_;
    webrtc::test::UdpTransport* socket_transport_;
};

#endif /* defined(__avDemo__UdpTransportAPI__) */

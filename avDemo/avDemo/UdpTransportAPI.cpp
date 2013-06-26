//
//  UdpTransportAPI.cpp
//  avDemo
//
//  Created by chenjianjun on 13-6-19.
//  Copyright (c) 2013年 free. All rights reserved.
//

#include "UdpTransportAPI.h"

using namespace webrtc;
using namespace webrtc::test;

VoiceTransport::VoiceTransport(webrtc::VoENetwork* voe_network,
                               int channel):channel_(channel),voe_network_(voe_network)
{
    uint8_t socket_threads = 1;
    socket_transport_ = UdpTransport::Create(channel, socket_threads);
    voe_network_->RegisterExternalTransport(channel,*socket_transport_);
}

VoiceTransport::~VoiceTransport()
{
    voe_network_->DeRegisterExternalTransport(channel_);
    UdpTransport::Destroy(socket_transport_);
}

void VoiceTransport::IncomingRTPPacket(const int8_t* incoming_rtp_packet,
                                       const int32_t packet_length,
                                       const char* /*from_ip*/,
                                       const uint16_t /*from_port*/) {
    voe_network_->ReceivedRTPPacket(channel_, incoming_rtp_packet, packet_length);
}

void VoiceTransport::IncomingRTCPPacket(const int8_t* incoming_rtcp_packet,
                                        const int32_t packet_length,
                                        const char* /*from_ip*/,
                                        const uint16_t /*from_port*/)
{
    voe_network_->ReceivedRTCPPacket(channel_, incoming_rtcp_packet,
                                     packet_length);
}

int VoiceTransport::SetLocalReceiver(uint16_t rtp_port)
{
    int return_value = socket_transport_->InitializeReceiveSockets(this,
                                                                   rtp_port);
    if (return_value == 0) {
        return socket_transport_->StartReceiving(kViENumReceiveSocketBuffers);
    }
    return return_value;
}

int VoiceTransport::SetSendDestination(const char* ip_address,
                                       uint16_t rtp_port)
{
    return socket_transport_->InitializeSendSockets(ip_address, rtp_port);
}


VideoTransport::VideoTransport(ViENetwork* vie_network,int channel):channel_(channel),vie_network_(vie_network)
{
    uint8_t socket_threads = 1;
    socket_transport_ = UdpTransport::Create(channel, socket_threads);
    vie_network_->RegisterSendTransport(channel,*socket_transport_);
}

VideoTransport::~VideoTransport()
{
    vie_network_->DeregisterSendTransport(channel_);
    UdpTransport::Destroy(socket_transport_);
}

void VideoTransport::IncomingRTPPacket(const int8_t* incoming_rtp_packet,
                                       const int32_t packet_length,
                                       const char* /*from_ip*/,
                                       const uint16_t /*from_port*/)
{
    vie_network_->ReceivedRTPPacket(channel_, incoming_rtp_packet, packet_length);
}

void VideoTransport::IncomingRTCPPacket(const int8_t* incoming_rtcp_packet,
                                        const int32_t packet_length,
                                        const char* /*from_ip*/,
                                        const uint16_t /*from_port*/)
{
    vie_network_->ReceivedRTCPPacket(channel_, incoming_rtcp_packet,
                                     packet_length);
}

int VideoTransport::SetLocalReceiver(uint16_t rtp_port)
{
    int return_value = socket_transport_->InitializeReceiveSockets(this,
                                                                   rtp_port);
    if (return_value == 0) {
        return socket_transport_->StartReceiving(kViENumReceiveSocketBuffers);
    }
    return return_value;
}

int VideoTransport::SetSendDestination(const char* ip_address,
                                       uint16_t rtp_port)
{
    return socket_transport_->InitializeSendSockets(ip_address, rtp_port);
}

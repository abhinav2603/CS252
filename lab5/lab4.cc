#include <string>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/error-model.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
#include "ns3/enum.h"
#include "ns3/event-id.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/traffic-control-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

int totalVal,total_bytes_cbr=0,total_bytes_ftp=0;
int total_drops=0;
bool first_drop=true;

//NS_LOG_COMPONENT_DEFINE ("TcpComparision");

AsciiTraceHelper ascii;
Ptr<PacketSink> cbrSink, tcpSink;

double endTime = 5.0;
double startTimeFTP = 0.0, endTimeFTP   = 5.0;
double startTimeCBR = 0.0, endTimeCBR   = 5.0;

int
main (int argc, char *argv[])
{
    std::string prot = "TcpWestwood";
    bool tracing = false;
    uint32_t maxBytes = 0;

    Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpWestwood::GetTypeId ()));
    Config::SetDefault ("ns3::TcpWestwood::ProtocolType", EnumValue (TcpWestwood::WESTWOODPLUS));
    Config::SetDefault ("ns3::TcpWestwood::FilterType", EnumValue (TcpWestwood::TUSTIN));
    
    Config::SetDefault ("ns3::QueueBase::MaxSize", StringValue ("20p"));
    Config::SetDefault ("ns3::QueueBase::MaxSize", QueueSizeValue (QueueSize (QueueSizeUnit::PACKETS, 20)));
    int senderWindowSize = 19300;
    Config::SetDefault ("ns3::TcpSocket::InitialCwnd", UintegerValue(senderWindowSize)); // was 8
    Config::SetDefault ("ns3::TcpSocket::RcvBufSize", UintegerValue(45000)); // was 65355

    //NS_LOG_INFO ("Create nodes.");
    NodeContainer nodes;
    nodes.Create (2);
    NodeContainer n0n1 = NodeContainer (nodes.Get (0), nodes.Get (1));

    InternetStackHelper internet;
    internet.Install (nodes);

    //NS_LOG_INFO ("Create channels.");

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("8Mbps")); //1 MBps
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("20ms"));
    pointToPoint.SetQueue ("ns3::DropTailQueue"); // <-----------?????

    //<<<<<<<<<<<<<<<<<<<<<<<<<--------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    NetDeviceContainer d0d1;
    d0d1 = pointToPoint.Install (n0n1);   
    
    // We've got the "hardware" in place.  Now we need to add IP addresses.
    //NS_LOG_INFO ("Assign IP Addresses.");
    Ipv4AddressHelper ipv4;
    ipv4.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i0i1 = ipv4.Assign (d0d1);
     
    
    // Create router nodes, initialize routing database and set up the routing
    // tables in the nodes.
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    //NS_LOG_INFO ("Create Applications.");

    //<<<<<<<<<<<<<------------------------>>>>>>>>>>>>>>>>>
    //Installing CBR

    uint16_t cbrPort = 12345;
    OnOffHelper onOff ("ns3::UdpSocketFactory", InetSocketAddress (i0i1.GetAddress (1), cbrPort));
    std::string CBRdataRate= "800Kbps";
    onOff.SetConstantRate (DataRate (CBRdataRate));

    ApplicationContainer apps = onOff.Install (nodes.Get (0));
    apps.Start (Seconds (startTimeCBR));
    apps.Stop (Seconds (endTimeCBR));

    PacketSinkHelper sinkCBR ("ns3::UdpSocketFactory",
                          Address (InetSocketAddress (Ipv4Address::GetAny (), cbrPort)));
    apps = sinkCBR.Install (nodes.Get (1));
    apps.Start (Seconds (startTimeCBR));
    apps.Stop (Seconds (endTimeCBR));

    //Installing FTP

    uint16_t ftpSenderPort = 12344;
    BulkSendHelper source ("ns3::TcpSocketFactory",InetSocketAddress (i0i1.GetAddress(1), ftpSenderPort));
    maxBytes = 50*1024*1024;
    source.SetAttribute ("MaxBytes", UintegerValue (maxBytes));

    ApplicationContainer sourceApps = source.Install (nodes.Get (0));
    sourceApps.Start (Seconds (startTimeFTP));
    sourceApps.Stop (Seconds (endTimeFTP));

    PacketSinkHelper sinkFTP ("ns3::TcpSocketFactory",
                            InetSocketAddress (Ipv4Address::GetAny(), ftpSenderPort));//<----------Why not the below one?
//       PacketSinkHelper sinkFTP ("ns3::TcpSocketFactory",InetSocketAddress (i0i1.GetAddress(1), ftpSenderPort));
    ApplicationContainer sinkApps = sinkFTP.Install (nodes.Get (1));
    sinkApps.Start (Seconds (startTimeFTP));
    sinkApps.Stop (Seconds (endTimeFTP));


    //<<<<<<<<<<<<----------------------------------------->>>>>>>>>>>>>>
    //Running the simulation....
    if (tracing)
    {
        AsciiTraceHelper ascii;
        pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("tcp-comparision.tr"));
        pointToPoint.EnablePcapAll ("tcp-comparision", true);
    }

    //NS_LOG_INFO ("Run Simulation.");

    Ptr<FlowMonitor> flowMonitor;
    FlowMonitorHelper flowHelper;
    flowMonitor = flowHelper.InstallAll();

    Simulator::Stop (Seconds (endTime));
    Simulator::Run ();
    
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper.GetClassifier ());
    std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats ();

   std::cout << std::endl << "====================== Flow monitor statistics ====================== " << std::endl;
   std::cout << std::endl ;
   //for loop with iterator is only required if there are multiple flows, leaving it here just to show how it is used
   
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin (); iter != stats.end (); ++iter)
    {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);

          std::cout << "Flow ID: " << iter->first << "\nSrc Addr: " << t.sourceAddress << " ----- Dst Addr: " << t.destinationAddress << std::endl;
          std::cout << std::endl ;
          std::cout << "  Tx Bytes\t\t" << iter->second.txBytes << std::endl;
          std::cout << "  Tx Packets\t\t" << iter->second.txPackets << std::endl;
          std::cout << "  Rx Bytes\t\t" << iter->second.rxBytes << std::endl;
          std::cout << "  Rx Packet\t\t" << iter->second.rxPackets << std::endl;
          std::cout << "  Input Load\t\t" << iter->second.txBytes * 8.0 / (iter->second.timeLastTxPacket.GetSeconds () - iter->second.timeFirstTxPacket.GetSeconds ()) / 1024 << " Kbps" << std::endl;
          std::cout << "  Observed Throughput\t" << iter->second.rxBytes * 8.0 / (iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds()) / 1024  << " Kbps" << std::endl;
          std::cout << "  Mean delay\t\t" << iter->second.delaySum.GetSeconds () / iter->second.rxPackets << std::endl;
          std::cout << "  Mean jitter\t\t" << iter->second.jitterSum.GetSeconds () / (iter->second.rxPackets - 1) << std::endl;
    }

    std::cout << std::endl << std::endl ;
    flowMonitor->SerializeToXmlFile("data.flowmon", true, true);
    Simulator::Destroy ();
    //NS_LOG_INFO ("Done.");

}
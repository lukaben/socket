using System;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.IO;
using System.Diagnostics;
using System.Text;

public class Client
{
    static void Main(string[] args)
    {

        TcpClient tcpSocket;
        bool status = true;
        try
        {
            tcpSocket = new TcpClient("localhost", 8080);
            Console.WriteLine("Connected to the Server");
            Console.WriteLine("Enter 'close' to end");
        }
        catch (Exception e)
        {
            Console.WriteLine("Cannot connect to the server");
            Console.WriteLine(e.ToString());
            return;
        }
        NetworkStream networkStream = tcpSocket.GetStream();
        StreamReader streamReader = new StreamReader(networkStream);
        StreamWriter streamWriter = new StreamWriter(networkStream);


        string filePath = @"C:\Users\Lukas\Documents\Visual Studio 2017\Projects\client_cs_cp\vid.mp4";
        Console.WriteLine("Sending File...");
        //DateTime startTime = DateTime.Now;
        //Stopwatch sw = Stopwatch.StartNew();

        FileStream fs = new FileStream(filePath, FileMode.Open, FileAccess.Read);
        byte[] data = new byte[fs.Length];
        int bytesToRead = (int)fs.Length;
        int bytesRead = 0;
        while (bytesToRead > 0)
        {
            int n = fs.Read(data, bytesRead, bytesToRead);
            if (n == 0)
                break;
            bytesRead += n;
            bytesToRead -= n;
        }
        bytesToRead = data.Length;


        byte[] length = BitConverter.GetBytes(data.Length);
        Stopwatch sw = new Stopwatch();

        sw.Start();
        tcpSocket.Client.Send(length);
        tcpSocket.Client.Send(data);
        //tcpSocket.Client.SendFile(filePath);
        string r = streamReader.ReadLine();
        sw.Stop();

        Console.WriteLine("Elapsed: " + sw.Elapsed + " ms: " + sw.ElapsedMilliseconds);
        //Ping pingSender = new Ping();
        //PingOptions options = new PingOptions(64, true);
        //PingReply reply = pingSender.Send("localhost",10000,data);
        //Console.WriteLine("RoundTrip time: " + reply.RoundtripTime.ToString() + "ms");


        Console.WriteLine("File sent");
        Console.WriteLine("Press any to continue...");
        Console.ReadLine();

        try
        {
            string clientString = "";
            string serverString = "";

            while (status)
            {
                Console.Write("Client:");
                clientString = Console.ReadLine();
                if ((clientString == "close") || (clientString == "CLOSE"))
                {
                    status = false;
                    streamWriter.WriteLine("close");
                    streamWriter.Flush();
                }
                if ((clientString != "close") && (clientString != "CLOSE"))
                {
                    streamWriter.WriteLine(clientString);
                    streamWriter.Flush();
                    serverString = streamReader.ReadLine();
                    Console.WriteLine("Server:" + serverString);
                }
            }
        }
        catch (Exception e)
        {
            Console.WriteLine("Cannot read from the server");
            Console.WriteLine(e.ToString());
        }
        streamReader.Close();
        networkStream.Close();
        streamWriter.Close();
    }
}


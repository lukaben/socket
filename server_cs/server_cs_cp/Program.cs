using System;
using System.Net.Sockets;
using System.IO;
using System.Diagnostics;
using System.Text;

public class Server
{

    public static void Main()
    {
        try
        {
            bool status = true;
            string serverString = "";
            string clientString = "";

            TcpListener tcpListener = new TcpListener(8080);
            tcpListener.Start();
            Console.WriteLine("Starting Server");
            Socket socketForClient = tcpListener.AcceptSocket();
            Console.WriteLine("Client Connected");

            NetworkStream networkStream = new NetworkStream(socketForClient);
            StreamWriter streamWriter = new StreamWriter(networkStream);
            StreamReader streamReader = new StreamReader(networkStream);

            var outputFile = File.Create(@"E:\S\i4i\GS\vid.mp4");

            Console.WriteLine("Receiving the file...");
            byte[] buffer = new Byte[1024];
            int bytesRead;

            byte[] dataSize = new Byte[10];
            int num = socketForClient.Receive(dataSize);
            int size = BitConverter.ToInt32(dataSize, 0);

            int total = 0;
            while (total < size)
            {
                bytesRead = socketForClient.Receive(buffer);
                outputFile.Write(buffer, 0, bytesRead);
                total += bytesRead;
                //Console.WriteLine(bytesRead);
            }
            streamWriter.WriteLine("");
            streamWriter.Flush();
            Console.WriteLine("File Received");


            while (status)
            {
                if (socketForClient.Connected)
                {
                    serverString = streamReader.ReadLine();
                    Console.WriteLine("Client:" + serverString);
                    if ((serverString == "close"))
                    {
                        status = false;
                        streamReader.Close();
                        networkStream.Close();
                        streamWriter.Close();
                        return;
                    }
                    Console.Write("Server:");
                    clientString = Console.ReadLine();
                    streamWriter.WriteLine(clientString);
                    streamWriter.Flush();
                }
            }
            streamReader.Close();
            networkStream.Close();
            streamWriter.Close();
            socketForClient.Close();
        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
        }
    }
}
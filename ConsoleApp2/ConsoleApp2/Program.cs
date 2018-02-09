using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using NewsAPI;
using NewsAPI.Constants;
using NewsAPI.Models;
using MySql.Data;
using MySql.Data.MySqlClient;
using System.Threading;
namespace ConsoleApp2
{
    class Program
    {
        public static void Main(string[] args)
        {
            var apiClient = new NewsApiClient(args[0]);
            
            MySqlConnection connection = new MySqlConnection();
            string connstring = string.Format("Server=aggregator.ddns.net; database={0}; UID=cs441backendAccess; password=t4back441", "aggregator");
            connection = new MySqlConnection(connstring);
            connection.Open();
            
            if (connection.State.ToString() == "Closed")
                Console.WriteLine("Connection Opened");
            while (true)
            {
                for (int i = 0; i <= 5; i++)
                {
                    newsResponse temp = new newsResponse(apiClient, i, connection);
                    if (temp.Status == Statuses.Error)
                        Console.WriteLine("Error pulling from API");
                }
                Console.WriteLine("Sleeping...");
                Thread.Sleep(1800000);
            }
           connection.Close();
        }
       
    }
}

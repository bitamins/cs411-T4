using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MySql.Data;
using MySql.Data.MySqlClient;
using NewsAPI;
using NewsAPI.Constants;
using NewsAPI.Models;

namespace ConsoleApp2
{
    
    public class newsResponse
    {   
        //array of categorys used to convert from 
        string[] responses = { "Business", "Entertainment", "Health", "Science", "Sports", "Technology" };
        ArticlesResult newsResp;
        Statuses status;
        Categories category;
        public Statuses Status
        {
            get{ return status; }
        }

        //constructor for newsResponse object sets the category to cat(0 = Business, 1 = Entertainment, 2 = Health, 3 = Science, 4 = Sports
        //5 = Technology) this is used to make a request to newsApi for a specific news category and writing the result to the correct table in the database
        //after the request response is received the response is written to the database specified by the argument "connection"
        public newsResponse(NewsApiClient cl, int cat, MySqlConnection connection)
        {
            category = (Categories)cat; 
            getResponse(cl);
            addToDb(connection);    
        }


        //fetches a response from the NewsApi and saves the response to a NewsApi.Models.ArticleResult object which is essentially just a list of articles
        public void  getResponse(NewsApiClient cl)
        {
            var response = cl.GetTopHeadlines(new NewsAPI.Models.TopHeadlinesRequest
            {
                Category = category,
                Language = Languages.EN,
                Country = Countries.US,
                PageSize = 50
            });
            newsResp = response;
            status = response.Status;
        }
        //loops through the list of Articles in this object and writes each article to the sql server specified by the argument "connection"
        public void addToDb(MySqlConnection connection)
        {
            foreach(var art in newsResp.Articles)
            {
                writeToDb(art, connection); 
            }
        }

        //function writes to the table specified by article.category, creates a prepared query and than executes that query on the argument "connection"
        public void writeToDb(NewsAPI.Models.Article article, MySqlConnection connection)
        {
            
            try
            {
                //use the title of the article to get a hashcode of an entry, this will be used as the primary key(Prevents duplicate stories from being added to db)
                int id = article.Title.GetHashCode();
                MySqlCommand query = connection.CreateCommand();

                //This chunk is used to create a prepared sql statement(safety feature), string format is used to allow this function to write to any table given a specific category
                query.CommandText = String.Format("INSERT INTO {0} (id, title, source, author, url, image, pubDate, description) VALUES (@id, @title, @source, @author, @url, @image, @pubDate, @description)", responses[(int)category]);
                query.Parameters.AddWithValue("@id", id);
                query.Parameters.AddWithValue("@title", article.Title);
                query.Parameters.AddWithValue("@source", article.Source.Name);
                query.Parameters.AddWithValue("@author", article.Author);
                query.Parameters.AddWithValue("@url", article.Url);
                query.Parameters.AddWithValue("@image", article.UrlToImage);
                query.Parameters.AddWithValue("@pubDate", article.PublishedAt);
                query.Parameters.AddWithValue("@description", article.Description);

                //execute query to sql database specified by the variable "connection"
                var temp = query.ExecuteReader();
                //close the reader so a new one can eventually be opened on the same connection
                temp.Close();
            }
            //an sql exception was thrown, write the error message to the console
            catch(MySqlException e)
            {
                Console.WriteLine(e.Message);
            }
            
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Xml;

namespace WeatherApp
{
    class WeatherData
    {
        public WeatherData(string City)
        {
            city = City;
        }

        public string city;
        private double city_temperature;

        public string City { get => city; set => city = value; }
        public double CityTemperature
        {
            get => city_temperature;
            set => city_temperature = value;
        }

        public void CheckWeather()
        {
            // Grab temperature from API using WeatherAPI object
            WeatherAPI DataAPI = new WeatherAPI(City);
            CityTemperature = DataAPI.GetCityTemperature();
        }
    }

    class WeatherAPI
    {
        public WeatherAPI(string city)
        {
            // Constructor that sets URL and retrieves XML from API
            SetWeatherURL(city);
            weatherXML = GetXML(weatherURL);
        }

        public double GetCityTemperature()
        {
            // Grab temperature value from XML document
            XmlNode tempNode = weatherXML.SelectSingleNode("//temperature");
            XmlAttribute tempValue = tempNode.Attributes["value"];
            string tempString = tempValue.Value;
            return double.Parse(tempString);
        }

        // API KEY is only avaiable to individuals who sign up for account
        private const string APIKEY = "API KEY";
        private string weatherURL;
        private XmlDocument weatherXML;

        private void SetWeatherURL(string city)
        {
            weatherURL = "http://api.openweathermap.org/data/2.5/weather/q?={city}"
                         + "&mode=xml&units=metric&APPID={APIKEY}";
        }

        private XmlDocument GetXML(string currentURL)
        {
            // Dispose of WebClient once XML is retrieved
            using (WebClient client = new WebClient())
            {
                string xmlContent = client.DownloadString(currentURL);
                XmlDocument xmlDoc = new XmlDocument();
                xmlDoc.LoadXml(xmlContent);
                return xmlDoc;
            }
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Console.Write("For which city would you like to check the weather?  ");
            string userCity = Console.ReadLine();
            WeatherData CityWeather = new WeatherData(userCity);
            CityWeather.CheckWeather();
            Console.WriteLine(CityWeather.CityTemperature);
            string input = Console.ReadLine();
        }
    }
}

package ex3

import org.apache.spark.SparkConf

import org.apache.spark.sql.SQLContext
import org.apache.spark.sql.Column
import org.apache.spark.sql.Row
import org.apache.spark.sql.DataFrame
import org.apache.spark.sql.Dataset

import org.apache.spark.sql.types.StructType
import org.apache.spark.sql.types.{ArrayType, StringType, StructField}
import org.apache.spark.sql.types.DataType
import org.apache.spark.sql.functions.col
import org.apache.spark.sql.functions.{sum, min, max, asc, desc, udf}

import org.apache.spark.sql.functions.explode
import org.apache.spark.sql.functions.array
import org.apache.spark.sql.SparkSession

import com.databricks.spark.xml._
import org.apache.spark.storage.StorageLevel
import org.apache.spark.streaming.{Seconds, StreamingContext}

import java.lang.Thread
import sys.process._

import org.apache.log4j.Logger
import org.apache.log4j.Level

case class Sales(year:Int,euros:Int)

object ex3Main extends App {

  // Suppress the log messages
  Logger.getLogger("org").setLevel(Level.OFF)

  // https://docs.databricks.com/spark/latest/data-sources/read-csv.html
	val spark = SparkSession.builder()
                          .appName("ex3")
                          .config("spark.driver.host", "localhost")
                          .master("local")
                          .getOrCreate()

  spark.conf.set("spark.sql.shuffle.partitions", "5")


  // Task 1: File "data/sales_data_sample.csv" contains sales data of a retailer.
  //         Study the file and read the data into DataFrame retailerDataFrame.
  val  retailerDataFrame: DataFrame = spark
    .read
    .option("inferSchema","true")
    .option("header","true")
    .csv("data/sales_data_sample.csv")
    .cache()

  retailerDataFrame.printSchema()

  // Task 2: Find the best 10 selling days. That is the days for which QUANTITYORDERED * PRICEEACH
  //         gets the highest values.                                         
  val best10DaysDF = retailerDataFrame
    .groupBy("ORDERDATE")
    .sum("SALES")
    .orderBy(desc("sum(SALES)"))

  best10DaysDF.printSchema()
  best10DaysDF.show(10)

  best10DaysDF.foreach(println)


  // Task 3: In the streaming version of the analysis, the streaming data will be added 
  //         into the directory streamingData. Data is streaming in as files similar to the one
  //         in the directory data.
  //
  //         Change static retailerDataFrame to operate with streaming data.
  //         Hint: Spark cannot infer the schema of streaming data, so you have to give it explicitly. 
  
  val  retailerStreamingDF: DataFrame =  spark
    .readStream
    .schema(retailerDataFrame.schema)
    .load("streamingData/*.csv")
  
  // Task 4: The classes that takes a type just like a parameter are known to be Generic 
  //         Classes in Scala. Dataset is an example of a generic class. Actually, DataFrame is
  //         a type alias for Dataset[Row], where Row is given as a type parameter. Declare your 
  //         own case class Sales with two members: year and euros of type integer. The
  //         class must be declared before this object (ex3Main).
  
  //         Then instantiate a Dataset[Sales] and query for the sales on 2019 and 
  //         the year of maximum sales.


  import spark.implicits._
  val salesList = List(Sales(2016, 100), Sales(2017, 15), Sales(2018, 1000), Sales(2019, 50))
  val salesDS: Dataset[Sales] = spark.createDataset(salesList)

  salesDS.printSchema()
  salesDS.show(5)

  val sales2019: Sales = salesDS.where(col("year") === 2019).take(1)(0)
  println(f"Sales for 2019 is ${sales2019.euros}")


  val maximumSales: Sales = salesDS
    .sort(desc("euros"))
    .take(1)(0)
  println(f"Maximum sales: year = ${maximumSales.year}, euros = ${maximumSales.euros}")

  // Extra Task 5: Find the best 10 selling days in the streaming data

  val best10DaysDFStreaming = retailerStreamingDF
    .groupBy("ORDERDATE")
    .sum("SALES")
    .orderBy(desc("sum(SALES)"))


  // Extra Task 6: Test your solution by writing the 10 best selling days to stdout
  //         whenever the DataFrame changes

  best10DaysDFStreaming.writeStream
    .outputMode("complete")
    .format("console")
    .start()

  // You can test your solution by uncommenting the following code snippet. The loop
  // adds a new CSV file to the directory streamingData every 5th second. If you rerun the test
  // remove all the CSV files from the directory streamingData.

  /*
  val repoFiles = "ls streamingDataRepo" !!

  for(f <- repoFiles.split("\n")){
	  val cmd = f"cp streamingDataRepo/${f} streamingData/${f}.csv"
    val res = cmd !!

    Thread.sleep(5000)
  }
  */

}

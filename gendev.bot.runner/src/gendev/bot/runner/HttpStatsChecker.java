package gendev.bot.runner;

import java.io.IOException;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.net.http.HttpResponse.BodyHandlers;
import java.util.LinkedList;
import java.util.concurrent.ExecutionException;

public class HttpStatsChecker {

    public static void main(String[] args) throws IOException, InterruptedException, ExecutionException {
        long min = 10000, max = 0;
        LinkedList<Long> last100 = new LinkedList<>();
        
        long count = 0;
        while(true) {
            Thread.sleep(50);
            long t = System.currentTimeMillis();
            request();
            t = System.currentTimeMillis() - t;
            
            last100.add(t);
            if (last100.size() > 100) {
                last100.removeFirst();
            }
            min = Math.min(min, t);
            max = Math.max(max, t);
            long avg = last100.stream().reduce(0L, Long::sum)/last100.size();
            
            if (count++ > 1000) {
                System.out.format("min = %d, max = %d, avg100 = %d\n", min, max, avg);
                count = 0;
                min = 1000;
                max = 0;
            }
        }
    }

    private static void request() throws InterruptedException, ExecutionException {

        // create a client
        HttpClient client = HttpClient.newHttpClient();

        // create a request
        HttpRequest request = HttpRequest.newBuilder(URI.create("http://" + RobotRunner.HOST_IP + ":8080/robot?distance=" + (int)Math.random() * 100))
                .header("accept", "application/json").build();

        // use the client to send the request
        HttpResponse<String> response;
        try {
            response = client.send(request, BodyHandlers.ofString());
//            System.out.println(response.body());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


}

# Autonomous Vehicle Status Distributer (AVeStDi) #
The AVeStDi daemon receives positional data from sensors like gps-receivers which are typically attached to an autonomous vehicle (AV). It stores the most recent data, informs AVs about other nearby vehicles and provides relational data like distance and direction.
Every AV is identified by an uuid.

__Important: AVeStDi is meant only for experimental purposes. Central security features like authentication, signatures, and encryption are missing.__

## TCP-Socket on 6156: ##
Connect via telnet 127.0.0.1 6156

#### VERS: returns date of build as string ####
Answer:

    AVeSti Daemon Version: Mar  8 2024 14:59:30

#### QUIT: closes connection ####
Answer:

    EXIT bye

#### LIST: returns json array of all known AVs #####
Answer:

    [{"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603001",
    "timestamp":"105709519199112",position":[53.11933910,8.19172210,8.400]},
    {"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603002",
    "timestamp":"105709519203016",position":[53.11562820,8.19115920,8.400]},
    {"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603003",
    "timestamp":"105709519204104",position":[53.11734130,8.18872230,8.400]}]

#### PUTS {json string}: pushes a given json string to publish or updates AV ####
Request:

    PUTS {"sensor":"gps","uuid":"0f389c46-ea13-45e0-b6a7-af282a603001","date":"20220227","time":"014307692","position":[53.1193391,8.1917221,8.4]}

Answer:

    OK

#### PUTL {json string}: like PUTS, but server answers with json array of all known AVs, including distance and direction towards the pushed AV ####
Request: 

    PUTL {"sensor":"gps","uuid":"0f389c46-ea13-45e0-b6a7-af282a603001","date":"20220227","time":"014307692","position":[53.1193391,8.1917221,8.4]}

Answer:

    [{"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603002",
    "timestamp":"105709519203016",position":[53.11562820,8.19115920,8.400]",
    distance":414.6966,"direction":5.2142},
    {"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603003",
    "timestamp":"105709519204104",position":[53.11734130,8.18872230,8.400]",
    distance":299.6184,"direction":42.0924}]
      
## WEB-Socket on 8096: ##

#### GET on /*: returns data of build as string ####
Request:

    curl http://127.0.0.1:8096

Answer:

    Answer: AVeSti Daemon Version: Mar  8 2024 14:59:30
    
#### PUT <json string> on /*: pushes a given json string to publish or update AV ####
Request:

     curl -X PUT -H "Content-Type: application/json" -d '{"sensor":"gps","uuid":"0f389c46-ea13-45e0-b6a7-af282a603001","date":"20220227","time":"014307692","position":[53.1193391,8.1917221,8.4]}' http://127.0.0.1:8096

Answer:

    OK

#### GET on /list: returns json array of all known AVs ####
Request:

    curl http://127.0.0.1:8096/list

Answer:

    [{"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603001",
    "timestamp":"105709519199112",position":[53.11933910,8.19172210,8.400]},
    {"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603002",
    "timestamp":"105709519203016",position":[53.11562820,8.19115920,8.400]},
    {"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603003",
    "timestamp":"105709519204104",position":[53.11734130,8.18872230,8.400]}]


#### PUT <json string> on /list: like PUT on /*, but server answers with json array of all known AVs, including distance and direction towards the pushed AV ####
Request:

    curl -X PUT -H "Content-Type: application/json" -d '{"sensor":"gps","uuid":"0f389c46-ea13-45e0-b6a7-af282a603001","date":"20220227","time":"014307692","position":[53.1193391, 8.1917221,8.4]}' http://127.0.0.1:8096/list

Answer:

    [{"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603002",
    "timestamp":"105709519203016",position":[53.11562820,8.19115920,8.400]",
    distance":414.6966,"direction":5.2142},
    {"type":"ground","uuid":"0f389c46-ea13-45e0-b6a7-af282a603003",
    "timestamp":"105709519204104",position":[53.11734130,8.18872230,8.400]",
    distance":299.6184,"direction":42.0924}]

## json PUT Parameters: ##
| Key          | Data-Format | Mandatory | Example          | Description                           |
|--------------|-------------|-----------|------------------|---------------------------------------|
| type         | String      |    [x]    | "ground"         | Type of vehicle                       |
| sensor       | String      |    [x]    | "gps"            | Must be "gps"                         |
| date         | String      |    [x]    | "20240101"       | YYYYMMDD                              |
| time         | String      |    [x]    | "235959001"      | HHMMSSMMM(illisecs)                   |
| position     | Float-Array |    [o]    | [53.11,8.18,8.4] | absolute position [LAT,LON,ALT]       |
| std_dev      | Float-Array |    [o]    | [0.22,0.34,1.5]  | standard deviation of [LAT,LON,ALT]   |
| velocity     | Float-Array |    [o]    | [0.6,0.01,0.0]   | local velocity [X,Y,Z]                |
| orientation  | Float-Array |    [o]    | [0.6,0.01,0.0]   | absolute orientation [YAW,PITCH,ROLL] |
| rotation     | Float-Array |    [o]    | [11.3,0.0,0.0]   | angular velocity [YAW,PITCH,ROLL]     |
| acceleration | Float-Array |    [o]    | [0.1,-0.001,0.0] | linear acceleration [X,Y,Z]           |
| ang_accel    | Float-Array |    [o]    | [-1.6,0.0,0.0]   | angular acceleration [YAW,PITCH,ROLL] |
| perimeter    | Float       |    [o]    | 2000.3           | max distance in metre of returned AVs |
| max_age      | Float       |    [o]    | 60               | max age in seconds of returned AVs    |

Unknown parameters are simply ignored by AVeStDi.

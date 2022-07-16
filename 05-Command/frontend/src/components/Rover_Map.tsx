import {useState,useEffect,useRef,useCallback} from 'react'
import axios from 'axios'
var x_previous: number;
var y_previous: number;
var rover_x: number;
var rover_y: number;
var alien_x: number;
var alien_y: number;
var radar_x: number;
var radar_y: number;
var color: string;
var alienlocate_x: string[];
var alienlocate_y: string[];


var arr_alien_color:string[] = [] 

var canvas_initial_offset: number;     // ensure @ (0,0) image of rover can fully displayed
var canvas_initial_offset_x: number; 
var canvas_initial_offset_y: number; 
canvas_initial_offset=0;
canvas_initial_offset_x=-17;
canvas_initial_offset_y=-17;

var route_x: number[] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
var route_y: number[] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
var route_x_previous: number[] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
var route_y_previous: number[] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
var route_len: number;

/* ------------- force useEffect to render (useful because useRef will prevent the change being noticed) ------------- */
export function useForceUpdate() {
  const [, setTick] = useState(0);
  const update = useCallback(() => {
    setTick(tick => tick + 1);
  }, [])
  return update;
}

/* -------------------------------------------------  define canvas  ------------------------------------------------- */
interface CanvasProps {
    width: number;
    height: number;
}

/* ----------------------------------------------  main function  start  ---------------------------------------------- */
const Map = ({ width, height }: CanvasProps) => {

    const forceUpdate = useForceUpdate();

    const [roverlocatex, setRoverlocateX] = useState<number[]>([0]);
    const [roverlocatey, setRoverlocateY] = useState<number[]>([0]);

    const [alienlocatex, setAlienlocateX] = useState<number[]>([0]);
    const [alienlocatey, setAlienlocateY] = useState<number[]>([0]);
    const [aliencolor, setAliencolor] = useState<string[]>([]);

    const [radarlocatex, setRadarlocateX] = useState<number[]>([0]);
    const [radarlocatey, setRadarlocateY] = useState<number[]>([0]);

    const canvasRef = useRef<HTMLCanvasElement>(null);

    useEffect(() => {

        const intervalId = setInterval(() => {

        /* --------------- NEW Route Planning NEW --------------- */
        const FetchRoute = async () => {
            try {
                const res = await axios.get('https://us-central1-rover-back-end.cloudfunctions.net/route')
                for (let i in res){
                    if((res.data)['color']!==arr_alien_color[i]){
                        route_x.push((res.data)[i]['x-axis'])
                        route_y.push((res.data)[i]['y-axis'])
                        setAliencolor((res.data)['color'])
                        console.log("other_alien_data", (res.data)['color'])
                    }
                }
            } 
            catch (error) {
                console.log("error!!! @ FetchRoute @ map", error)
            }
        }

        /* ------------------------------   obtain data from database - rover location   ------------------------------ */
        const FetchRoverData = async () => {
            try {
                const res = await axios.get('https://us-central1-rover-back-end.cloudfunctions.net/rover_1')
                setRoverlocateX((res.data)[0])
                setRoverlocateY((res.data)[1])
            } 
            catch (error) {
                console.log("error @ FetchRoverData @ map", error)
            }
        }

        /* ------------------------------   obtain data from database - alien location   ------------------------------ */
        const FetchAlienData = async () => {
            try {
                const res1 = await axios.get('https://us-central1-rover-back-end.cloudfunctions.net/new_alien_app')
                // setAlienlocateX((res1.data)['x-axis'])
                // setAlienlocateY((res1.data)['y-axis'])
                // setAliencolor((res1.data)['color'])
                // console.log("check_alien_data", (res1.data)['color'])

                if(arr_alien_color.length==0){
                    setAlienlocateX((res1.data)["x_value"])
                    setAlienlocateY((res1.data)["y_value"])
                    setAliencolor((res1.data)["color"])
                    console.log("first_alien_data", (res1.data)["color"])
                }
                else{
                    for (let i in arr_alien_color){
                        if((res1.data)['color']!==arr_alien_color[i]){
                            setAlienlocateX((res1.data)["x_value"])
                            setAlienlocateY((res1.data)["y_value"])
                            setAliencolor((res1.data)["color"])
                            console.log("other_alien_data", (res1.data)["color"])
                        }
                    }
                }

            }
            catch (error) {
                console.log("error @ FetchAlienData @ map", error)
            }
        }

        const FetchRadarData = async () => {
            try {
                const res1 = await axios.get('https://us-central1-rover-back-end.cloudfunctions.net/radar_loc')
                setRadarlocateX(res1.data)
                // console.log("check_map_radar_data", (res1.data)[0]['x-axis'])
                // console.log("check_map_radar_data", res1.data["x-axis"])
            }
            catch (error) {
                console.log("error @ FetchRoverData @ map", error)
            }
        }

        /* ----------------------   obtain coordinates where image is displayed - rover location   ---------------------- */
        var roverlocate_x = roverlocatex.toString().split("")
        var roverlocate_y = roverlocatey.toString().split("")
        if (rover_x!==Math.abs(parseInt(roverlocate_x.join(""))*1.5) || rover_y!==Math.abs(parseInt(roverlocate_y.join(""))*1.5)){
            x_previous=rover_x
            y_previous=rover_y}
        rover_x = canvas_initial_offset_x+Math.abs(parseInt(roverlocate_x.join(""))*1.5)    //+5: make sure full image/circle is displayed
        rover_y = canvas_initial_offset_y+Math.abs(parseInt(roverlocate_y.join(""))*1.5)
        console.log("rover data: ", rover_x, ", ", rover_y)

        /* ----------------------   obtain coordinates where image is displayed - alien location   ---------------------- */
        if (alienlocatex !== null) {        
        alienlocate_x = alienlocatex.toString().split("")
        alienlocate_y = alienlocatey.toString().split("")
        alien_x = canvas_initial_offset+Math.abs(parseInt(alienlocate_x.join(""))*1.5)
        alien_y = canvas_initial_offset+Math.abs(parseInt(alienlocate_y.join(""))*1.5)
        var alien_color = aliencolor.toString().split("")
        color = alien_color.join("")}


        /* -------------------------   radar   ------------------------*/
        // if(radarlocatex[0]==1){
        // var radarlocate_x = radarlocatex.toString().split("")
        // var radarlocate_y = radarlocatey.toString().split("")
        // radar_x = parseInt(radarlocate_x.join(""))
        // radar_y = canvas_initial_offset+parseInt(radarlocate_y.join(""))
        //console.log("radar location to be drawn: ", radar_x, radar_y)
        //}

        /* -------------------------   clear previous image & draw new image - rover location   -------------------------*/
        if (canvasRef.current) {
            //console.log("dubug");
            var canvas = canvasRef.current;
            var map_drawing = canvas.getContext('2d');  
            var tmp_1 = {     //use to resize image
                width: 0,
                height:0 }

            var img = new Image();
            img.src = '/image/Rover.png';
            tmp_1.width=Math.floor(img.width * 0.076)
            tmp_1.height=Math.floor(img.height * 0.06)
            img.onload = function() {
                    map_drawing!.clearRect(x_previous, y_previous, tmp_1.width, tmp_1.height);
                    console.log('clear: x_pre-',x_previous, 'y_pre-', y_previous)  // for debug
                    map_drawing!.drawImage(img, rover_x, rover_y, tmp_1.width,  tmp_1.height);
                    console.log('draw: x-',rover_x, 'y-', rover_y);  // for debug
            };
            // console.log("debug ! ! ! : ", img.width, " * ",img.height)

            /* ------------------------   radar   -------------------------*/
            var tmp_2 = {width: 0, height:0 }
            var img_radar = new Image();
            img_radar.src = '/image/Infrastructure.png';
            tmp_2.width=Math.floor(img_radar.width * 0.09)
            tmp_2.height=Math.floor(img_radar.height * 0.09)
            img_radar.onload = function() {
            //console.log("dubug !!! [0]", radarlocatex[0])
            if(radarlocatex["x_value"]===230){
                    map_drawing!.drawImage(img_radar, 230, 230, tmp_2.width,  tmp_2.height);
                    // console.log('radar_draw: x-',radar_x, 'y-', radar_y);  // for debug
            }
            //console.log("debug !!! ", radarlocatex)
            };
           
            /* ------------------------   clear previous image & draw new image - alien location   ------------------------ */
            if(map_drawing && (alien_x!==canvas_initial_offset || alien_y!==canvas_initial_offset)){
                map_drawing.beginPath();
                if (color==='BUILDING'){
                    map_drawing.rect(alien_x, alien_y, 5, 5);     //rectangular OR bulding drawing
                    map_drawing.fillStyle = 'white'
                } else {
                    map_drawing.arc(alien_x, alien_y, 7, 0, 2*Math.PI);     //circle drawing
                    map_drawing.fillStyle = color
                }
                map_drawing.fill();
                // map_drawing.stroke();
            }   else {console.log('error loading contents')}
        }       
        FetchRadarData()
        FetchRoverData()
        FetchAlienData()
        }, 1000);
        return () => clearInterval(intervalId); //This is important
    } );

    /*     grid     */

    return (
        <div>
            <canvas ref={canvasRef} height={height} width={width} />
        </div>
            // <svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg">
            //     <defs>
            //         <pattern id="grid" width="45" height="45" patternUnits="userSpaceOnUse">
            //             <path d="M 45 0 L 0 0 0 45" fill="none" stroke="white" stroke-width="2"/>
            //         </pattern>
            //         <pattern id="grid" width="180" height="180" patternUnits="userSpaceOnUse">
            //         <rect width="180" height="180" fill="url(#smallGrid)"/>
            //             <path d="M 180 0 L 0 0 0 180" fill="none" stroke="gray" stroke-width="0"/>
            //         </pattern>
            //     </defs>
            //     <rect width="100%" height="100%" fill="url(#grid)" />
            // </svg>

    );
};

/* -------------------------------------------------  canvas size  ------------------------------------------------- */
Map.defaultProps = {
    width: 540,
    height: 360,
};

export default Map;
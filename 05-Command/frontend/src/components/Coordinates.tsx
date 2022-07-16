import Paper from '@mui/material/Paper';
import Grid from '@mui/material/Grid';
import Avatar from '@mui/material/Avatar';
import './coordinates.css';
import Locate_Alien from './Locate_Alien';
import Locate_Rover from './Locate_Rover';
import Radar from './Radar';

// var arr_alien: string[][] = [];

var arr_alien:string[][] = [["0","0","0"],["0","0","0"],["0","0","0"],["0","0","0"],["0","0","0"],["0","0","0"]]  

interface alien{
    x: string
    y: string
    color: string}

const Coordinates = () => {
  /*                  -----------get location of aliens-----------                  */
    var xaxis = (Locate_Alien())
    var radar = (Radar())
    //console.log("location of radar recieved: ", radar)

  /*                  ------------get location of rover------------                  */

    var roverlocate = Locate_Rover()
    var roverlocatex = roverlocate[0]
    var roverlocatey = roverlocate[1]

    for (let i in xaxis){
        let value = xaxis[i];
        if (arr_alien[0][0]==='0'){
            console.log('alien array inserted: ',value["color"])
            arr_alien[0][0]=value["x_value"]
            arr_alien[0][1]=value["y_value"]
            arr_alien[0][2]=value["color"]
            console.log("debug alien locate: ", arr_alien[0])
        }
        else if (arr_alien[1][0]==='0'){
            if(value['color']!==arr_alien[0][2]){
            arr_alien[1][0]=value["x_value"]
            arr_alien[1][1]=value["y_value"]
            arr_alien[1][2]=value["color"]}}
        else if (arr_alien[2][0]==='0'){
            if(value['color']!==arr_alien[0][2] && value["color"]!==arr_alien[1][2]){
            arr_alien[2][0]=value["x_value"]
            arr_alien[2][1]=value["y_value"]
            arr_alien[2][2]=value["color"]}}
        else if (arr_alien[3][0]==='0'){
            if(value['color']!==arr_alien[0][2] && value["color"]!==arr_alien[1][2] && value["color"]!==arr_alien[2][2]){
            arr_alien[3][0]=value["x_value"]
            arr_alien[3][1]=value["y_value"]
            arr_alien[3][2]=value["color"]}}   
        else if (arr_alien[4][0]==='0'){
            if(value['color']!==arr_alien[0][2] && value["color"]!==arr_alien[1][2] && value["color"]!==arr_alien[2][2] && value["color"]!==arr_alien[3][2]){
            arr_alien[4][0]=value["x_value"]
            arr_alien[4][1]=value["y_value"]
            arr_alien[4][2]=value["color"]}}  
        else if (arr_alien[5][0]==='0'){
            if(value['color']!==arr_alien[0][2] && value["color"]!==arr_alien[1][2] && value["color"]!==arr_alien[2][2] && value["color"]!==arr_alien[3][2] && value["color"]!==arr_alien[4][2]){
            arr_alien[5][0]=value["x_value"]
            arr_alien[5][1]=value["y_value"]
            arr_alien[5][2]=value["color"]}}         
        else{   }
        }

/*         ------------display locations in x & y coordinates------------           */
    return (
        <Grid>
            {/* Rover_position */}
            <Paper elevation={24} sx={{ height:100, marginLeft: 8, marginRight: 30, marginBottom: 2, background:'rgba(255,255,255,0.7)'}}>
            <Grid container alignItems = "center" direction="row" sx={{marginLeft: 1}}>
                  <Grid>
                <Avatar alt="Rover" src="/image/Rover.png" sx={{marginTop:0.5}}/>
                </Grid> 
                
                <Grid sx={{marginLeft: 1}}>
                <h2>Rover Position: </h2>
                </Grid>
            
            </Grid>
            <Grid sx={{marginLeft: 1, marginBottom: 5}}>
                <h2> x-axis: {roverlocatex} | y-axis: {roverlocatey}</h2>
                </Grid>
            </Paper>

        {/* alien_position */}
        <Paper elevation={24} sx={{ height:279, marginLeft: 8, marginRight: 30, marginBottom: 2, background:'rgba(255,255,255,0.8)'}}>
            <Grid container alignItems = "center" direction="row" sx={{marginLeft: 1}}>
                <Grid>
                <Avatar alt="Alien" src="/image/Alien.png" />
                </Grid>
                <Grid sx={{marginLeft: 1}}>
                <h2>Alien Position: </h2>
                </Grid>   
            </Grid>
            <Grid sx={{marginLeft: 1}}>
                {xaxis.length > 0 ? (
                <div>
                        {arr_alien[0][0]=='0'?(<h2> </h2>):(<h2>x-axis: {arr_alien[0][0]} | y-axis: {arr_alien[0][1]} | color: {arr_alien[0][2]}</h2>)}
                        {arr_alien[1][0]=='0'?(<h2> </h2>):(<h2>x-axis: {arr_alien[1][0]} | y-axis: {arr_alien[1][1]} | color: {arr_alien[1][2]}</h2>)}
                        {arr_alien[2][0]=='0'?(<h2> </h2>):(<h2>x-axis: {arr_alien[2][0]} | y-axis: {arr_alien[2][1]} | color: {arr_alien[2][2]}</h2>)}

                        {arr_alien[3][0]=='0'?(<h2> </h2>):(<h2>x-axis: {arr_alien[2][0]} | y-axis: {arr_alien[2][1]} | color: {arr_alien[2][2]}</h2>)}
                        {arr_alien[4][0]=='0'?(<h2> </h2>):(<h2>x-axis: {arr_alien[2][0]} | y-axis: {arr_alien[2][1]} | color: {arr_alien[2][2]}</h2>)}
                        {arr_alien[5][0]=='0'?(<h2> </h2>):(<h2>x-axis: {arr_alien[2][0]} | y-axis: {arr_alien[2][1]} | color: {arr_alien[2][2]}</h2>)}

                </div>
                ):( <h2 className="loading">Loading... </h2>)}
            </Grid> 
            </Paper>

            <Paper elevation={24} sx={{ height:80, marginLeft: 8, marginRight: 30, background:'rgba(255,255,255,0.8)'}}>
            <Grid container alignItems = "center" direction="row" sx={{marginLeft: 1}}>
                  <Grid>
                <Avatar alt="Radar" src="/image/Infrastructure.png" />
                </Grid> 
                
                <Grid sx={{marginLeft: 1}}>
                <h2>Power Infrastructure Position: </h2>
                </Grid>
            
            </Grid>
            <Grid sx={{marginLeft: 1, marginBottom: 5}}>
            {radar ? (
                    <h2> x-axis: {radar["x_value"]} | y-axis: {radar["y_value"]}</h2>
                ):( <h2 className="loading">Loading... </h2>)}
                </Grid>
            </Paper>
            
      </Grid>

    )
}

export default Coordinates
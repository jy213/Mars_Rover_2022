import Grid from '@mui/material/Grid';
import Button from '@mui/material/Button';


export default function Database() {

  return (
    <Grid 
    container 
    justifyContent="space-evenly" 
    alignItems = "space-evenly" 
    direction="column" 

    sx={{marginTop:16, marginLeft:20, width: 30}}>

    <Button variant="contained" sx={{fontFamily: "Nunito", fontSize:"1.5rem", marginBottom: 2, height: 50, width: 300}} 
            onClick={() => window.open('https://rover-back-end-default-rtdb.europe-west1.firebasedatabase.app', '_blank')}>Database</Button>

    <Button variant="contained" sx={{fontFamily: "Nunito", fontSize:"1.5rem", marginBottom: 2, height: 50}} 
            onClick={() => window.open('https://console.firebase.google.com/u/1/project/rover-back-end/functions/list', '_blank')}>Data Logs</Button>
    
    <Button variant="contained" sx={{fontFamily: "Nunito", fontSize:"1.5rem", marginBottom: 2, height: 50}} 
            onClick={() => window.open('https://console.firebase.google.com/u/0/project/rover-back-end/overview', '_blank')}>Overall Usage</Button>

    </Grid>
  )
}

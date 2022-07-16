import Grid from '@mui/material/Grid';
import IconButton from '@mui/material/IconButton';
import SignalCellularAltIcon from '@mui/icons-material/SignalCellularAlt';
import Typography from '@mui/material/Typography';
import Popover from '@mui/material/Popover';
import PopupState, { bindTrigger, bindPopover } from 'material-ui-popup-state';
import BatteryCharging90Icon from '@mui/icons-material/BatteryCharging90';
import Battery from './Battery_Level'


export default function Signal() {
  var a=Battery(97)
  return (
<Grid
  container
  direction="row"
  justifyContent="flex-end"
  alignItems="center"
  marginTop={1.5}
  marginLeft={3.5}
>
    <IconButton>
        <SignalCellularAltIcon sx={{color:'#fff',height:70, width:70}}/>
    </IconButton>

    {/* --------------------               pop-up window version of battery level           ---------------------- */}
    <PopupState variant="popover" popupId="demo-popup-popover">
      {(popupState) => (
        <div>

    <IconButton sx={{variant:'contained', color:'#76ff03'} } {...bindTrigger(popupState)}>
        <BatteryCharging90Icon sx={{height:70, width:70}}/>
    </IconButton>

    <Popover
            {...bindPopover(popupState)}
            anchorOrigin={{
              vertical: 'bottom',
              horizontal: 'center',
            }}
            transformOrigin={{
              vertical: 'top',
              horizontal: 'center',
            }}
          >
            <Typography sx={{ p: 2 }}>{a}%</Typography>
          </Popover>
        </div>
      )}
    </PopupState>
  
</Grid>
)
}


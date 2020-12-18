
fade_time <- 0.35;

// ============================================================================
//   UNDERGROUND
// ============================================================================

if(! ("underground" in state)){
	state.underground <- false;
  print("[DEBUG] Underground state initialized\n");
}

function go_underground(under){
  Underground_cover.fade(under ? 0 : 1, fade_time);
  Land_foreground_1.fade(under ? 0 : 1, fade_time);
  Land_foreground_2.fade(under ? 0 : 1, fade_time);
  Land_foreground_3.fade(under ? 0 : 1, fade_time);
  Land_foreground_4.fade(under ? 0 : 1, fade_time);
  Underground_mask.fade(under ? 1 : 0, fade_time);
  Underground_darkness.fade(under ? 1 : 0, fade_time);
  state.underground <- under;
}

go_underground(state.underground);



// ============================================================================
//   ROAD FORKS
// ============================================================================

if(! ("normal_fitr" in state)){
	state.normal_fitr <- false;
  print("[DEBUG] 'Fork in the Road' normal road initialized\n");
}

if(! ("normal_iv" in state)){
	state.normal_iv <- false;
  print("[DEBUG] 'Fork in the Road' normal road initialized\n");
}

function toggle_normal_road(tilemap, enabled){
tilemap.set_solid(enabled);
}

toggle_normal_road(normal_fitr, state.normal_fitr);
toggle_normal_road(normal_iv, state.normal_iv);


// ============================================================================
//   SECRET ROADS
// ============================================================================

if(! ("secret_fitr" in state)){
	state.secret_fitr <- false;
  print("[DEBUG] 'A Fork in the Road' secret road fork initialized\n");
}

if(! ("secret_iv" in state)){
	state.secret_iv <- false;
  print("[DEBUG] 'Icy Valley' secret road fork initialized\n");
}

function toggle_secret_road(tilemap, enabled){
  tilemap.fade(enabled ? 1 : 0, fade_time);
  tilemap.set_solid(enabled);
}

toggle_secret_road(secret_fitr, state.secret_fitr);
toggle_secret_road(secret_iv, state.secret_iv);

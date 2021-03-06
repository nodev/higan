auto VCE::read(uint3 address) -> uint8 {
  uint8 data = 0xff;

  if(address == 0x04) {
    //CTR
    data.bit(0,7) = cram.read(cram.address).bit(0,7);
    return data;
  }

  if(address == 0x05) {
    //CTR
    data.bit(0) = cram.read(cram.address).bit(8);
    cram.address++;
    return data;
  }

  return data;
}

auto VCE::write(uint3 address, uint8 data) -> void {
  if(address == 0x00) {
    //CR
    if(data.bit(0,1) == 0) io.clock = 4;
    if(data.bit(0,1) == 1) io.clock = 3;
    if(data.bit(0,1) == 2) io.clock = 2;
    if(data.bit(0,1) == 3) io.clock = 3;
    io.extraLine = data.bit(2);
    io.grayscale = data.bit(7);
    return;
  }

  if(address == 0x02) {
    //CTA
    cram.address.bit(0,7) = data.bit(0,7);
    return;
  }

  if(address == 0x03) {
    //CTA
    cram.address.bit(8) = data.bit(0);
    return;
  }

  if(address == 0x04) {
    //CTW
    cram.write(cram.address, 0, data.bit(0,7));
    return;
  }

  if(address == 0x05) {
    //CTW
    cram.write(cram.address, 1, data.bit(0));
    cram.address++;
    return;
  }
}

auto VCE::power() -> void {
  for(auto& data : cram.memory) data = 0;
  cram.address = 0;

  io = {};
}

auto VCE::CRAM::read(uint9 address) -> uint9 {
  return memory[address];
}

auto VCE::CRAM::write(uint9 address, uint1 a0, uint8 data) -> void {
  if(a0 == 0) memory[address].bit(0,7) = data.bit(0,7);
  if(a0 == 1) memory[address].bit(8)   = data.bit(0);
}

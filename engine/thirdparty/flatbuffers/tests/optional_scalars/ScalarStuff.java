// automatically generated by the FlatBuffers compiler, do not modify

package optional_scalars;

import com.google.flatbuffers.BaseVector;
import com.google.flatbuffers.BooleanVector;
import com.google.flatbuffers.ByteVector;
import com.google.flatbuffers.Constants;
import com.google.flatbuffers.DoubleVector;
import com.google.flatbuffers.FlatBufferBuilder;
import com.google.flatbuffers.FloatVector;
import com.google.flatbuffers.LongVector;
import com.google.flatbuffers.StringVector;
import com.google.flatbuffers.Struct;
import com.google.flatbuffers.Table;
import com.google.flatbuffers.UnionVector;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

@SuppressWarnings("unused")
public final class ScalarStuff extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_22_12_06(); }
  public static ScalarStuff getRootAsScalarStuff(ByteBuffer _bb) { return getRootAsScalarStuff(_bb, new ScalarStuff()); }
  public static ScalarStuff getRootAsScalarStuff(ByteBuffer _bb, ScalarStuff obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public static boolean ScalarStuffBufferHasIdentifier(ByteBuffer _bb) { return __has_identifier(_bb, "NULL"); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public ScalarStuff __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public byte justI8() { int o = __offset(4); return o != 0 ? bb.get(o + bb_pos) : 0; }
  public boolean hasMaybeI8() { return 0 != __offset(6); }
  public byte maybeI8() { int o = __offset(6); return o != 0 ? bb.get(o + bb_pos) : 0; }
  public byte defaultI8() { int o = __offset(8); return o != 0 ? bb.get(o + bb_pos) : 42; }
  public int justU8() { int o = __offset(10); return o != 0 ? bb.get(o + bb_pos) & 0xFF : 0; }
  public boolean hasMaybeU8() { return 0 != __offset(12); }
  public int maybeU8() { int o = __offset(12); return o != 0 ? bb.get(o + bb_pos) & 0xFF : 0; }
  public int defaultU8() { int o = __offset(14); return o != 0 ? bb.get(o + bb_pos) & 0xFF : 42; }
  public short justI16() { int o = __offset(16); return o != 0 ? bb.getShort(o + bb_pos) : 0; }
  public boolean hasMaybeI16() { return 0 != __offset(18); }
  public short maybeI16() { int o = __offset(18); return o != 0 ? bb.getShort(o + bb_pos) : 0; }
  public short defaultI16() { int o = __offset(20); return o != 0 ? bb.getShort(o + bb_pos) : 42; }
  public int justU16() { int o = __offset(22); return o != 0 ? bb.getShort(o + bb_pos) & 0xFFFF : 0; }
  public boolean hasMaybeU16() { return 0 != __offset(24); }
  public int maybeU16() { int o = __offset(24); return o != 0 ? bb.getShort(o + bb_pos) & 0xFFFF : 0; }
  public int defaultU16() { int o = __offset(26); return o != 0 ? bb.getShort(o + bb_pos) & 0xFFFF : 42; }
  public int justI32() { int o = __offset(28); return o != 0 ? bb.getInt(o + bb_pos) : 0; }
  public boolean hasMaybeI32() { return 0 != __offset(30); }
  public int maybeI32() { int o = __offset(30); return o != 0 ? bb.getInt(o + bb_pos) : 0; }
  public int defaultI32() { int o = __offset(32); return o != 0 ? bb.getInt(o + bb_pos) : 42; }
  public long justU32() { int o = __offset(34); return o != 0 ? (long)bb.getInt(o + bb_pos) & 0xFFFFFFFFL : 0L; }
  public boolean hasMaybeU32() { return 0 != __offset(36); }
  public long maybeU32() { int o = __offset(36); return o != 0 ? (long)bb.getInt(o + bb_pos) & 0xFFFFFFFFL : 0L; }
  public long defaultU32() { int o = __offset(38); return o != 0 ? (long)bb.getInt(o + bb_pos) & 0xFFFFFFFFL : 42L; }
  public long justI64() { int o = __offset(40); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public boolean hasMaybeI64() { return 0 != __offset(42); }
  public long maybeI64() { int o = __offset(42); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public long defaultI64() { int o = __offset(44); return o != 0 ? bb.getLong(o + bb_pos) : 42L; }
  public long justU64() { int o = __offset(46); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public boolean hasMaybeU64() { return 0 != __offset(48); }
  public long maybeU64() { int o = __offset(48); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public long defaultU64() { int o = __offset(50); return o != 0 ? bb.getLong(o + bb_pos) : 42L; }
  public float justF32() { int o = __offset(52); return o != 0 ? bb.getFloat(o + bb_pos) : 0.0f; }
  public boolean hasMaybeF32() { return 0 != __offset(54); }
  public float maybeF32() { int o = __offset(54); return o != 0 ? bb.getFloat(o + bb_pos) : 0f; }
  public float defaultF32() { int o = __offset(56); return o != 0 ? bb.getFloat(o + bb_pos) : 42.0f; }
  public double justF64() { int o = __offset(58); return o != 0 ? bb.getDouble(o + bb_pos) : 0.0; }
  public boolean hasMaybeF64() { return 0 != __offset(60); }
  public double maybeF64() { int o = __offset(60); return o != 0 ? bb.getDouble(o + bb_pos) : 0.0; }
  public double defaultF64() { int o = __offset(62); return o != 0 ? bb.getDouble(o + bb_pos) : 42.0; }
  public boolean justBool() { int o = __offset(64); return o != 0 ? 0!=bb.get(o + bb_pos) : false; }
  public boolean hasMaybeBool() { return 0 != __offset(66); }
  public boolean maybeBool() { int o = __offset(66); return o != 0 ? 0!=bb.get(o + bb_pos) : false; }
  public boolean defaultBool() { int o = __offset(68); return o != 0 ? 0!=bb.get(o + bb_pos) : true; }
  public byte justEnum() { int o = __offset(70); return o != 0 ? bb.get(o + bb_pos) : 0; }
  public boolean hasMaybeEnum() { return 0 != __offset(72); }
  public byte maybeEnum() { int o = __offset(72); return o != 0 ? bb.get(o + bb_pos) : 0; }
  public byte defaultEnum() { int o = __offset(74); return o != 0 ? bb.get(o + bb_pos) : 1; }

  public static int createScalarStuff(FlatBufferBuilder builder,
      byte justI8,
      byte maybeI8,
      byte defaultI8,
      int justU8,
      int maybeU8,
      int defaultU8,
      short justI16,
      short maybeI16,
      short defaultI16,
      int justU16,
      int maybeU16,
      int defaultU16,
      int justI32,
      int maybeI32,
      int defaultI32,
      long justU32,
      long maybeU32,
      long defaultU32,
      long justI64,
      long maybeI64,
      long defaultI64,
      long justU64,
      long maybeU64,
      long defaultU64,
      float justF32,
      float maybeF32,
      float defaultF32,
      double justF64,
      double maybeF64,
      double defaultF64,
      boolean justBool,
      boolean maybeBool,
      boolean defaultBool,
      byte justEnum,
      byte maybeEnum,
      byte defaultEnum) {
    builder.startTable(36);
    ScalarStuff.addDefaultF64(builder, defaultF64);
    ScalarStuff.addMaybeF64(builder, maybeF64);
    ScalarStuff.addJustF64(builder, justF64);
    ScalarStuff.addDefaultU64(builder, defaultU64);
    ScalarStuff.addMaybeU64(builder, maybeU64);
    ScalarStuff.addJustU64(builder, justU64);
    ScalarStuff.addDefaultI64(builder, defaultI64);
    ScalarStuff.addMaybeI64(builder, maybeI64);
    ScalarStuff.addJustI64(builder, justI64);
    ScalarStuff.addDefaultF32(builder, defaultF32);
    ScalarStuff.addMaybeF32(builder, maybeF32);
    ScalarStuff.addJustF32(builder, justF32);
    ScalarStuff.addDefaultU32(builder, defaultU32);
    ScalarStuff.addMaybeU32(builder, maybeU32);
    ScalarStuff.addJustU32(builder, justU32);
    ScalarStuff.addDefaultI32(builder, defaultI32);
    ScalarStuff.addMaybeI32(builder, maybeI32);
    ScalarStuff.addJustI32(builder, justI32);
    ScalarStuff.addDefaultU16(builder, defaultU16);
    ScalarStuff.addMaybeU16(builder, maybeU16);
    ScalarStuff.addJustU16(builder, justU16);
    ScalarStuff.addDefaultI16(builder, defaultI16);
    ScalarStuff.addMaybeI16(builder, maybeI16);
    ScalarStuff.addJustI16(builder, justI16);
    ScalarStuff.addDefaultEnum(builder, defaultEnum);
    ScalarStuff.addMaybeEnum(builder, maybeEnum);
    ScalarStuff.addJustEnum(builder, justEnum);
    ScalarStuff.addDefaultBool(builder, defaultBool);
    ScalarStuff.addMaybeBool(builder, maybeBool);
    ScalarStuff.addJustBool(builder, justBool);
    ScalarStuff.addDefaultU8(builder, defaultU8);
    ScalarStuff.addMaybeU8(builder, maybeU8);
    ScalarStuff.addJustU8(builder, justU8);
    ScalarStuff.addDefaultI8(builder, defaultI8);
    ScalarStuff.addMaybeI8(builder, maybeI8);
    ScalarStuff.addJustI8(builder, justI8);
    return ScalarStuff.endScalarStuff(builder);
  }

  public static void startScalarStuff(FlatBufferBuilder builder) { builder.startTable(36); }
  public static void addJustI8(FlatBufferBuilder builder, byte justI8) { builder.addByte(0, justI8, 0); }
  public static void addMaybeI8(FlatBufferBuilder builder, byte maybeI8) { builder.addByte(1, maybeI8, 0); }
  public static void addDefaultI8(FlatBufferBuilder builder, byte defaultI8) { builder.addByte(2, defaultI8, 42); }
  public static void addJustU8(FlatBufferBuilder builder, int justU8) { builder.addByte(3, (byte) justU8, (byte) 0); }
  public static void addMaybeU8(FlatBufferBuilder builder, int maybeU8) { builder.addByte(4, (byte) maybeU8, (byte) 0); }
  public static void addDefaultU8(FlatBufferBuilder builder, int defaultU8) { builder.addByte(5, (byte) defaultU8, (byte) 42); }
  public static void addJustI16(FlatBufferBuilder builder, short justI16) { builder.addShort(6, justI16, 0); }
  public static void addMaybeI16(FlatBufferBuilder builder, short maybeI16) { builder.addShort(7, maybeI16, 0); }
  public static void addDefaultI16(FlatBufferBuilder builder, short defaultI16) { builder.addShort(8, defaultI16, 42); }
  public static void addJustU16(FlatBufferBuilder builder, int justU16) { builder.addShort(9, (short) justU16, (short) 0); }
  public static void addMaybeU16(FlatBufferBuilder builder, int maybeU16) { builder.addShort(10, (short) maybeU16, (short) 0); }
  public static void addDefaultU16(FlatBufferBuilder builder, int defaultU16) { builder.addShort(11, (short) defaultU16, (short) 42); }
  public static void addJustI32(FlatBufferBuilder builder, int justI32) { builder.addInt(12, justI32, 0); }
  public static void addMaybeI32(FlatBufferBuilder builder, int maybeI32) { builder.addInt(13, maybeI32, 0); }
  public static void addDefaultI32(FlatBufferBuilder builder, int defaultI32) { builder.addInt(14, defaultI32, 42); }
  public static void addJustU32(FlatBufferBuilder builder, long justU32) { builder.addInt(15, (int) justU32, (int) 0L); }
  public static void addMaybeU32(FlatBufferBuilder builder, long maybeU32) { builder.addInt(16, (int) maybeU32, (int) 0L); }
  public static void addDefaultU32(FlatBufferBuilder builder, long defaultU32) { builder.addInt(17, (int) defaultU32, (int) 42L); }
  public static void addJustI64(FlatBufferBuilder builder, long justI64) { builder.addLong(18, justI64, 0L); }
  public static void addMaybeI64(FlatBufferBuilder builder, long maybeI64) { builder.addLong(19, maybeI64, 0L); }
  public static void addDefaultI64(FlatBufferBuilder builder, long defaultI64) { builder.addLong(20, defaultI64, 42L); }
  public static void addJustU64(FlatBufferBuilder builder, long justU64) { builder.addLong(21, justU64, 0L); }
  public static void addMaybeU64(FlatBufferBuilder builder, long maybeU64) { builder.addLong(22, maybeU64, 0L); }
  public static void addDefaultU64(FlatBufferBuilder builder, long defaultU64) { builder.addLong(23, defaultU64, 42L); }
  public static void addJustF32(FlatBufferBuilder builder, float justF32) { builder.addFloat(24, justF32, 0.0f); }
  public static void addMaybeF32(FlatBufferBuilder builder, float maybeF32) { builder.addFloat(25, maybeF32, 0f); }
  public static void addDefaultF32(FlatBufferBuilder builder, float defaultF32) { builder.addFloat(26, defaultF32, 42.0f); }
  public static void addJustF64(FlatBufferBuilder builder, double justF64) { builder.addDouble(27, justF64, 0.0); }
  public static void addMaybeF64(FlatBufferBuilder builder, double maybeF64) { builder.addDouble(28, maybeF64, 0.0); }
  public static void addDefaultF64(FlatBufferBuilder builder, double defaultF64) { builder.addDouble(29, defaultF64, 42.0); }
  public static void addJustBool(FlatBufferBuilder builder, boolean justBool) { builder.addBoolean(30, justBool, false); }
  public static void addMaybeBool(FlatBufferBuilder builder, boolean maybeBool) { builder.addBoolean(31, maybeBool, false); }
  public static void addDefaultBool(FlatBufferBuilder builder, boolean defaultBool) { builder.addBoolean(32, defaultBool, true); }
  public static void addJustEnum(FlatBufferBuilder builder, byte justEnum) { builder.addByte(33, justEnum, 0); }
  public static void addMaybeEnum(FlatBufferBuilder builder, byte maybeEnum) { builder.addByte(34, maybeEnum, 0); }
  public static void addDefaultEnum(FlatBufferBuilder builder, byte defaultEnum) { builder.addByte(35, defaultEnum, 1); }
  public static int endScalarStuff(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }
  public static void finishScalarStuffBuffer(FlatBufferBuilder builder, int offset) { builder.finish(offset, "NULL"); }
  public static void finishSizePrefixedScalarStuffBuffer(FlatBufferBuilder builder, int offset) { builder.finishSizePrefixed(offset, "NULL"); }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public ScalarStuff get(int j) { return get(new ScalarStuff(), j); }
    public ScalarStuff get(ScalarStuff obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}


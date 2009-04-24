// Generated by the protocol buffer compiler.  DO NOT EDIT!

#include "mesh.pb.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format_inl.h>

namespace mesh {

namespace {

const ::google::protobuf::Descriptor* Mesh_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Mesh_reflection_ = NULL;
const ::google::protobuf::Descriptor* Mesh_Layer_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Mesh_Layer_reflection_ = NULL;

}  // namespace


void protobuf_BuildDesc_mesh_2eproto_AssignGlobalDescriptors(const ::google::protobuf::FileDescriptor* file) {
  Mesh_descriptor_ = file->message_type(0);
  Mesh::default_instance_ = new Mesh();
  static const int Mesh_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Mesh, layers_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Mesh, indices_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Mesh, primitive_type_),
  };
  Mesh_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Mesh_descriptor_,
      Mesh::default_instance_,
      Mesh_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Mesh, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Mesh, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      sizeof(Mesh));
  Mesh_Layer_descriptor_ = Mesh_descriptor_->nested_type(0);
  Mesh_Layer::default_instance_ = new Mesh_Layer();
  static const int Mesh_Layer_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Mesh_Layer, name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Mesh_Layer, columns_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Mesh_Layer, rows_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Mesh_Layer, data_),
  };
  Mesh_Layer_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Mesh_Layer_descriptor_,
      Mesh_Layer::default_instance_,
      Mesh_Layer_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Mesh_Layer, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Mesh_Layer, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      sizeof(Mesh_Layer));
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Mesh_Layer_descriptor_, Mesh_Layer::default_instance_);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Mesh_descriptor_, Mesh::default_instance_);
  Mesh::default_instance_->InitAsDefaultInstance();
  Mesh_Layer::default_instance_->InitAsDefaultInstance();
}

void protobuf_BuildDesc_mesh_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  ::google::protobuf::DescriptorPool* pool =
    ::google::protobuf::DescriptorPool::internal_generated_pool();

  pool->InternalBuildGeneratedFile(
    "\n\nmesh.proto\022\004mesh\"\225\001\n\004Mesh\022 \n\006layers\030\001 "
    "\003(\0132\020.mesh.Mesh.Layer\022\017\n\007indices\030\002 \003(\r\022\026"
    "\n\016primitive_type\030\003 \002(\005\032B\n\005Layer\022\014\n\004name\030"
    "\001 \002(\t\022\017\n\007columns\030\002 \002(\005\022\014\n\004rows\030\003 \002(\005\022\014\n\004"
    "data\030\004 \003(\002", 170,
  &protobuf_BuildDesc_mesh_2eproto_AssignGlobalDescriptors);
}

// Force BuildDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_mesh_2eproto {
  StaticDescriptorInitializer_mesh_2eproto() {
    protobuf_BuildDesc_mesh_2eproto();
  }
} static_descriptor_initializer_mesh_2eproto_;


// ===================================================================

const ::std::string Mesh_Layer::_default_name_;



Mesh_Layer::Mesh_Layer()
  : ::google::protobuf::Message(),
    _cached_size_(0),
    name_(const_cast< ::std::string*>(&_default_name_)),
    columns_(0),
    rows_(0) {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

void Mesh_Layer::InitAsDefaultInstance() {}

Mesh_Layer::Mesh_Layer(const Mesh_Layer& from)
  : ::google::protobuf::Message(),
    _cached_size_(0),
    name_(const_cast< ::std::string*>(&_default_name_)),
    columns_(0),
    rows_(0) {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  MergeFrom(from);
}

Mesh_Layer::~Mesh_Layer() {
  if (name_ != &_default_name_) {
    delete name_;
  }
  if (this != default_instance_) {
  }
}

const ::google::protobuf::Descriptor* Mesh_Layer::descriptor() {
  if (Mesh_Layer_descriptor_ == NULL) protobuf_BuildDesc_mesh_2eproto();
  return Mesh_Layer_descriptor_;
}

const Mesh_Layer& Mesh_Layer::default_instance() {
  if (default_instance_ == NULL) protobuf_BuildDesc_mesh_2eproto();
  return *default_instance_;
}

Mesh_Layer* Mesh_Layer::default_instance_ = NULL;

Mesh_Layer* Mesh_Layer::New() const {
  return new Mesh_Layer;
}

const ::google::protobuf::Descriptor* Mesh_Layer::GetDescriptor() const {
  return descriptor();
}

const ::google::protobuf::Reflection* Mesh_Layer::GetReflection() const {
  if (Mesh_Layer_reflection_ == NULL) protobuf_BuildDesc_mesh_2eproto();
  return Mesh_Layer_reflection_;
}

// -------------------------------------------------------------------




Mesh::Mesh()
  : ::google::protobuf::Message(),
    _cached_size_(0),
    primitive_type_(0) {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

void Mesh::InitAsDefaultInstance() {}

Mesh::Mesh(const Mesh& from)
  : ::google::protobuf::Message(),
    _cached_size_(0),
    primitive_type_(0) {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  MergeFrom(from);
}

Mesh::~Mesh() {
  if (this != default_instance_) {
  }
}

const ::google::protobuf::Descriptor* Mesh::descriptor() {
  if (Mesh_descriptor_ == NULL) protobuf_BuildDesc_mesh_2eproto();
  return Mesh_descriptor_;
}

const Mesh& Mesh::default_instance() {
  if (default_instance_ == NULL) protobuf_BuildDesc_mesh_2eproto();
  return *default_instance_;
}

Mesh* Mesh::default_instance_ = NULL;

Mesh* Mesh::New() const {
  return new Mesh;
}

const ::google::protobuf::Descriptor* Mesh::GetDescriptor() const {
  return descriptor();
}

const ::google::protobuf::Reflection* Mesh::GetReflection() const {
  if (Mesh_reflection_ == NULL) protobuf_BuildDesc_mesh_2eproto();
  return Mesh_reflection_;
}

}  // namespace mesh
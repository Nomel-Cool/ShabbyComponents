#pragma once
#ifndef TOPO_NODE_H
#define TOPO_NODE_H

#include <vector>
#include <memory>
/// <summary>
/// ���������ڶ��������������ݽṹ������ͼ���ĳ���ڵ���
/// ��Ҫ������ڵ�ĳ������������
/// </summary>
namespace shabby
{
	template<typename T, size_t IN_DEGREE, size_t OUT_DEGREE>
	class TopoNode
	{
	public:
		/// <summary>
		/// �����ȷ��������һ��ָ���Խṹ���Ͷ�������
		/// </summary>
		TopoNode()
		{
			shptr_in.resize(IN_DEGREE);
			unique_in.resize(IN_DEGREE);
			shptr_out.resize(OUT_DEGREE);
			unique_out.resize(OUT_DEGREE);
		}
		/// <summary>
		/// �����ȷ���ṹ���ر����ʾͿ����ֶ�ָ��
		/// </summary>
		/// <param name="duplicate_edge">�Ƿ��ر�</param>
		TopoNode(bool duplicate_edge)
		{
			if (duplicate_edge)
			{
				shptr_in.resize(IN_DEGREE);
				shptr_out.resize(OUT_DEGREE);
			}
			else
			{
				unique_in.resize(IN_DEGREE);
				unique_out.resize(OUT_DEGREE);
			}
		}

		/// <summary>
		/// ��ȡ�������ṹ�ĵ�i���ӽڵ�
		/// </summary>
		/// <param name="i">����i</param>
		/// <returns>�����i��λ���нڵ㣬�򷵻ض�Ӧ�Ľڵ㣬������vector��at��������</returns>
		virtual std::shared_ptr<TopoNode<T, IN_DEGREE, OUT_DEGREE> > GetTheIthChildNodeShared(size_t i)
		{
			return shptr_out.at(i);
		}
		/// <summary>
		/// ��ȡ��������ṹ�ĵ�i���ӽڵ�
		/// </summary>
		/// <param name="i">����i</param>
		/// <returns>�����i��λ���нڵ㣬�򷵻ض�Ӧ�Ľڵ㣬������vector��at��������</returns>
		virtual std::unique_ptr<TopoNode<T, IN_DEGREE, OUT_DEGREE> > GetTheIthChildNodeUnique(size_t i)
		{
			return unique_out.at(i);
		}
		/// <summary>
		/// ��ȡ�������ṹ�ĵ�i�����ڵ�
		/// </summary>
		/// <param name="i">����i</param>
		/// <returns>�����i��λ���нڵ㣬�򷵻ض�Ӧ�Ľڵ㣬������vector��at��������</returns>
		virtual std::shared_ptr<TopoNode<T, IN_DEGREE, OUT_DEGREE> > GetTheIthParentNodeShared(size_t i)
		{
			return shptr_in.at(i);
		}
		/// <summary>
		/// ��ȡ��������ṹ�ĵ�i�����ڵ�
		/// </summary>
		/// <param name="i">����i</param>
		/// <returns>�����i��λ���нڵ㣬�򷵻ض�Ӧ�Ľڵ㣬������vector��at��������</returns>
		virtual std::unique_ptr<TopoNode<T, IN_DEGREE, OUT_DEGREE> > GetTheIthParentNodeUnique(size_t i)
		{
			return unique_in.at(i);
		}
		/// <summary>
		/// ���õ�i�������ӽڵ�
		/// </summary>
		/// <param name="out_node">����ĵ���ͼ�Ľڵ�</param>
		/// <param name="i">���ߵ���������</param>
		virtual void SetTheIthChildNode(std::unique_ptr<TopoNode<T, IN_DEGREE, OUT_DEGREE> >& out_node, size_t i)
		{
			unique_out.at(i) = std::move(out_node);
		}
		/// <summary>
		/// ���õ�i���ر��ӽڵ�
		/// </summary>
		/// <param name="out_node">����Ķ��ͼ�ڵ�</param>
		/// <param name="i">��ߵ���������</param>
		virtual void SetTheIthChildNode(std::shared_ptr<TopoNode<T, IN_DEGREE, OUT_DEGREE> >& out_node, size_t i)
		{
			shptr_out.at(i) = std::move(out_node);
		}
		/// <summary>
		/// ���õ�i�����߸��ڵ�
		/// </summary>
		/// <param name="in_node">����ĵ���ͼ�ڵ�</param>
		/// <param name="i">���ߵ���������</param>
		virtual void SetTheIthParentNode(std::unique_ptr<TopoNode<T, IN_DEGREE, OUT_DEGREE> >& in_node, size_t i)
		{
			unique_in.at(i) = std::move(in_node);
		}
		/// <summary>
		/// ���õ�i���ر߸��ڵ�
		/// </summary>
		/// <param name="in_node">������ر�ͼ�ڵ�</param>
		/// <param name="i">�رߵ���������</param>
		virtual void SetTheIthParentNode(std::shared_ptr<TopoNode<T, IN_DEGREE, OUT_DEGREE> >& in_node, size_t i)
		{
			shptr_in.at(i) = std::move(in_node);
		}
		/// <summary>
		/// ���ؽڵ�ֵ����
		/// </summary>
		/// <returns>�ڵ���������</returns>
		virtual T& GetData()
		{
			return Data;
		}
		/// <summary>
		/// Ĭ��T����ʹ��=����ִ�и�ֵ����������Ӧ�ø���
		/// </summary>
		/// <param name="data">���������ֵ</param>
		virtual void SetData(const T& data)
		{
			Data = data;
		}
		virtual ~TopoNode() {}
	private:
		T Data;
		std::vector<std::shared_ptr<TopoNode<T, IN_DEGREE, OUT_DEGREE> > > shptr_in, shptr_out; // ʹ�ù���ָ�����ʵ�������ر�ָ��ṹ
		std::vector<std::unique_ptr<TopoNode<T, IN_DEGREE, OUT_DEGREE> > > unique_in, unique_out; // ʹ�ö���ָ�����ʵ���������ر�ָ��ṹ����Ч�ʱ�shared_ptr��
	};
}

#endif // !TOPO_NODE_H
